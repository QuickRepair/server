#include "DatabaseConnection.h"
#include "QueryResult.h"
#include "../../Account/ContactInformation.h"
#include "OrderStateParameters.h"
#include "../OrderStateFactories/OrderUnreceivedStateFactory.h"
#include "../OrderStateFactories/OrderReceivedStateFactory.h"
#include "../OrderStateFactories/OrderStartRepairStateFactory.h"
#include "../OrderStateFactories/OrderEndRepairStateFactory.h"
#include "../OrderStateFactories/OrderFinishedStateFactory.h"
#include "../../Errors/DatabaseInternalError.h"
#include "../../Errors/QueryResultEmptyError.h"
#include "../../Errors/AccountAlreadyExistError.h"
#include "../../Errors/PasswordNotRightError.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <chrono>

using std::string;						using std::runtime_error;
using std::tuple;						using std::istringstream;
using std::ostringstream;				using std::shared_ptr;
using std::make_shared;					using std::list;
using std::vector;

MYSQL * DatabaseConnection::m_mysqlConnection;

DatabaseConnection::DatabaseConnection()
{
	m_mysqlConnection = mysql_init(nullptr);
}

DatabaseConnection::~DatabaseConnection()
{
	mysql_close(m_mysqlConnection);
}

void DatabaseConnection::connect(std::string databaseIp, std::string databaseName, std::string databaseUser,
								 std::string databasePassword, unsigned databasePort)
{
	if (!mysql_real_connect(m_mysqlConnection, databaseIp.data(),
							databaseUser.data(), databasePassword.data(), databaseName.data(), databasePort,
							"/run/mysqld/mysqld.sock", 0))
		throw DatabaseInternalError("DatabaseConnection connection error, " + string(mysql_error(m_mysqlConnection)));
}

DatabaseConnection &DatabaseConnection::getInstance()
{
	static DatabaseConnection m_instance = DatabaseConnection();
	return m_instance;
}

unsigned long DatabaseConnection::createOrder(unsigned long committerId, unsigned long acceptorId,
											  std::string applianceType, std::string detail)
{
	ostringstream ostr;
	ostr << "INSERT INTO orders (committer, acceptor, appliance_type, detail, current_state) VALUES ('" << committerId << "', '" << acceptorId << "', '" << applianceType << "', '" << detail << "', 0)";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Create order error, " + string(mysql_error(m_mysqlConnection)));

	query = "SELECT id FROM orders where id=@@IDENTITY";
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Get created order id, " + string(mysql_error(m_mysqlConnection)));
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	if(res.empty())
		throw QueryResultEmptyError("Get empty order id");
	unsigned long orderId = toUnsignedLong(res[0]);

	ostringstream os;
	os << "INSERT INTO order_states (order_id, date, state_name) VALUES ('" << orderId << "', now(), 'unreceived')";
	query = os.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Create order state error, " + string(mysql_error(m_mysqlConnection)));
	query = "SELECT id FROM orders where id=@@IDENTITY";
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Get created order id, " + string(mysql_error(m_mysqlConnection)));
	QueryResult state = mysql_store_result(m_mysqlConnection);
	auto stateRes = state.fetchRow();
	if(stateRes.empty())
		throw QueryResultEmptyError("Get empty order id");
	unsigned long stateId = toUnsignedLong(stateRes[0]);

	ostringstream o;
	o << "UPDATE orders SET current_state=" << stateId << " WHERE order_id=" << orderId;
	query = o.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Get created order id, " + string(mysql_error(m_mysqlConnection)));

	return orderId;
}

std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>>
	DatabaseConnection::queryOrderByAccountId(unsigned long id)
{
	ostringstream ostr;
	ostr << "SELECT * FROM orders WHERE committer='" << id << "' or acceptor='" << id << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order by id error, " + string(mysql_error(m_mysqlConnection)));

	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchAllRows();
	if(res.empty())
		throw QueryResultEmptyError("Query Order by id empty");

	vector<tuple<unsigned long, unsigned long, unsigned long, string, string, unsigned long>> ret;
	for(int i = 0; i < res.size(); /*empty*/)
	{
		unsigned long orderId = toUnsignedLong(res[i++]);
		unsigned long committer = toUnsignedLong(res[i++]);
		unsigned long acceptor = toUnsignedLong(res[i++]);
		string applianceType = res[i++];
		string detail = res[i++];
		unsigned long state = toUnsignedLong(res[i++]);
		ret.emplace_back(
				tuple<unsigned long, unsigned long, unsigned long, string, string, unsigned long>(
						orderId, committer, acceptor, applianceType, detail, state)
		);
	}
	return ret;
}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		DatabaseConnection::queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId)
{
	//query
	ostringstream ostr;
	ostr << "SELECT * FROM order_states WHERE order_id='" << orderId
		<< "' AND id='" << stateId << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order state by id, " + string(mysql_error(m_mysqlConnection)));

	//save result
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	if(res.empty())
		throw QueryResultEmptyError("Query Order state by id empty, ");

	OrderStateParameters parameters;

	unsigned long currentStateId = toUnsignedLong(res[1]);
	unsigned long lastStateId = toUnsignedLong(res[2]);
	std::chrono::system_clock::time_point date = toTimePoint(res[3]);
	double transaction = toDouble(res[4]);
	string type = res[5];

	parameters.date = date;
	parameters.range = AcceptableOrderPriceRange();
	parameters.transactionPrice = transaction;
	parameters.currentStateId = currentStateId;
	parameters.lastStateId = lastStateId;

	auto factory = findFactory(type);

	return tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>(factory, parameters);
}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
DatabaseConnection::queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState)
{
	ostringstream ostr;
	ostr << "SELECT * FROM order_states WHERE order_id='" << orderId << "' AND last_state='" << lastState << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order state by id, " + string(mysql_error(m_mysqlConnection)));

	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	std::shared_ptr<OrderStateAbstractFactory> factory = nullptr;
	OrderStateParameters parameters;
	if(res.empty())
		throw QueryResultEmptyError("Query Order state by id and last state empty");

	unsigned long stateId = toUnsignedLong(res[1]);
	unsigned long lastStateId = toUnsignedLong(res[2]);
	std::chrono::system_clock::time_point date = toTimePoint(res[3]);
	double transaction = toDouble(res[4]);
	string type = res[5];

	parameters.date = date;
	parameters.range = AcceptableOrderPriceRange();
	parameters.transactionPrice = transaction;
	parameters.currentStateId = stateId;
	parameters.lastStateId = lastStateId;

	factory = findFactory(type);

	return tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>(factory, parameters);
}

unsigned long DatabaseConnection::checkMerchantPasswordAndGetId(std::string account, std::string password)
{
	return checkPasswordAndGetId(std::move(account), std::move(password), "merchant");
}

unsigned long DatabaseConnection::checkCustomerPasswordAndGetId(std::string account, std::string password)
{
	return checkPasswordAndGetId(std::move(account), std::move(password), "customer");
}

unsigned long DatabaseConnection::checkPasswordAndGetId(std::string account, std::string password, std::string accountType)
{
	// query
	ostringstream ostr;
	ostr << "SELECT * FROM account WHERE account='" << account << "' AND password='" << password << "' AND type='" << accountType << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Check password, " + string(mysql_error(m_mysqlConnection)));

	// get result
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	if(res.empty())
		throw PasswordNotRightError("Not right password");
	unsigned long id = toUnsignedLong(res[0]);
	return id;
}

std::vector<std::tuple<>> DatabaseConnection::queryContactInfoByUserId(unsigned long userId)
{
	//TODO, read from Database
	return std::vector<std::tuple<>>();
}

void DatabaseConnection::createMerchantAccount(std::string account, std::string password)
{
	createAccount(std::move(account), std::move(password), "merchant");
}

void DatabaseConnection::createCustomerAccount(std::string account, std::string password)
{
	createAccount(std::move(account), std::move(password), "customer");
}

void DatabaseConnection::createAccount(std::string account, std::string password, std::string accountType)
{
	// does the user exist
	std::string query =  "SELECT * FROM account WHERE account='" + account + "' and type='" + accountType + "'";
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Create user, " + std::string(mysql_error(m_mysqlConnection)));
	QueryResult findResult = mysql_store_result(m_mysqlConnection);
	auto isExist = findResult.fetchRow();
	if(!isExist.empty())
		throw AccountAlreadyExistError("Faild to create, " + account + " already exist");

	// insert into database
	query = "INSERT INTO account (account, password, type) VALUES ('" + account + "', '" + password + "', '" + accountType + "')";
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Create user, " + std::string(mysql_error(m_mysqlConnection)));
}

void DatabaseConnection::updateMerchantAccountPassword(std::string account, std::string password)
{
	updateAccount(std::move(account), std::move(password), "merchant");
}

void DatabaseConnection::updateCustomerAccountPassword(std::string account, std::string password)
{
	updateAccount(std::move(account), std::move(password), "customer");
}

void DatabaseConnection::updateAccount(std::string account, std::string password, std::string accountType)
{
	ostringstream ostr;
	ostr << "UPDATE account SET password='" << password << "' WHERE account='" << account << "' AND type='" << accountType << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Update password, " + string(mysql_error(m_mysqlConnection)));
}

std::tuple<std::list<std::string>, int> DatabaseConnection::queryMerchantServiceType(unsigned long id)
{
	// query supported types
	ostringstream ostr;
	ostr << "SELECT max_service_distance FROM account WHERE id=" << id << " AND type='merchant'"
		<< " UNION SELECT type FROM merchant_service_type WHERE merchant_id=" << id;
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query merchant service type, " + string(mysql_error(m_mysqlConnection)));

	// get result
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchAllRows();

	list<string> supportedTypes;
	if(res.empty())
		throw QueryResultEmptyError("Find empty result for merchant service type");

	int maxDistance = toInt(res[0]);
	for(unsigned i = 1; i != res.size(); ++i)
		supportedTypes.push_back(res[i]);

	return tuple<list<string>, int>(supportedTypes, maxDistance);
}

std::tuple<std::string, std::string> DatabaseConnection::loadMerchant(unsigned long id)
{
	ostringstream ostr;
	ostr << "SELECT account,password FROM account WHERE id=" << id << " AND type='merchant'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Load merchant, " + string(mysql_error(m_mysqlConnection)));
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	if(res.empty())
		throw QueryResultEmptyError("Find empty result for merchant");
	return tuple<string, string>(res[0], res[1]);
}

std::tuple<std::string, std::string> DatabaseConnection::loadCustomer(unsigned long id)
{
	ostringstream ostr;
	ostr << "SELECT account,password FROM account WHERE id=" << id << " AND type='customer'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Load customer, " + string(mysql_error(m_mysqlConnection)));
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetchRow();
	if(res.empty())
		throw QueryResultEmptyError("Find empty result for customer");
	return tuple<string, string>(res[0], res[1]);
}

unsigned long DatabaseConnection::toUnsignedLong(std::string str)
{
	istringstream is(str);
	unsigned long ret{0};
	is >> ret;
	return ret;
}

int DatabaseConnection::toInt(std::string str)
{
	istringstream is(str);
	int ret{0};
	is >> ret;
	return ret;
}

double DatabaseConnection::toDouble(std::string str)
{
	istringstream is(str);
	double ret{0};
	is >> ret;
	return ret;
}

std::chrono::system_clock::time_point DatabaseConnection::toTimePoint(std::string str)
{
	std::time_t t{0};
	long int system_time_t_type{0};
	istringstream is(str);
	is >> system_time_t_type;
	t = system_time_t_type;
	return std::chrono::system_clock::from_time_t(t);
}

std::shared_ptr<OrderStateAbstractFactory> DatabaseConnection::findFactory(std::string orderType)
{
	shared_ptr<OrderStateAbstractFactory> factory;
	if(orderType == "unreceived")
		factory = make_shared<OrderUnreceivedStateFactory>();
	else if(orderType == "received")
		factory = make_shared<OrderReceivedStateFactory>();
	else if(orderType == "startRepair")
		factory = make_shared<OrderStartRepairStateFactory>();
	else if(orderType == "endRepair")
		factory = make_shared<OrderEndRepairStateFactory>();
	else if(orderType == "finished")
		factory = make_shared<OrderFinishedStateFactory>();

	return factory;
}
