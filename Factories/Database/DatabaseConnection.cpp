#include "DatabaseConnection.hpp"
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

using std::string;			using std::runtime_error;
using std::tuple;			using std::istringstream;
using std::ostringstream;	using std::shared_ptr;
using std::make_shared;

MYSQL * DatabaseConnection::m_mysqlConnection;

DatabaseConnection::DatabaseConnection()
{
	m_mysqlConnection = mysql_init(nullptr);
}

DatabaseConnection::~DatabaseConnection()
{
	mysql_close(m_mysqlConnection);
}

void DatabaseConnection::conenct(std::string databaseIp, std::string databaseName, std::string databaseUser,
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

std::tuple<ContactInformation, std::string, unsigned long, unsigned long>
	DatabaseConnection::queryOrderById(unsigned long orderId)
{
	ostringstream ostr;
	ostr << "SELECT * FROM HAROrder WHERE id='" << orderId << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order by id error, " + string(mysql_error(m_mysqlConnection)));

	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetch_a_row();
	if(res.empty())
		throw QueryResultEmptyError("Query Order by id empty");

	unsigned long id = toUnsignedLong(res[0]);
	string detail = res[1];
	unsigned long currentState = toUnsignedLong(res[2]);
	return tuple<ContactInformation, std::string, unsigned long, unsigned long>{ContactInformation(), detail, id, currentState};
}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		DatabaseConnection::queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId)
{
	//query
	ostringstream ostr;
	ostr << "SELECT * FROM OrderState WHERE orderId='" << orderId
		<< "' AND stateId='" << stateId << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order state by id, " + string(mysql_error(m_mysqlConnection)));

	//save result
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetch_a_row();
	if(res.empty())
		throw QueryResultEmptyError("Query Order state by id empty, ");

	OrderStateParameters parameters;
	double priceLow = toDouble(res[3]);
	double priceHigh = toDouble(res[4]);
	std::chrono::system_clock::time_point date = toTimePoint(res[0]);
	double transaction = toDouble(res[2]);
	unsigned long currentStateId = toUnsignedLong(res[6]);
	unsigned long lastStateId = toUnsignedLong(res[7]);
	parameters.date = date;
	parameters.range = AcceptableOrderPriceRange(priceLow, priceHigh);
	parameters.transactionPrice = transaction;
	parameters.currentStateId = currentStateId;
	parameters.lastStateId = lastStateId;

	string type = res[5];
	auto factory = findFactory(type);

	return tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>(factory, parameters);
}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
DatabaseConnection::queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState)
{
	ostringstream ostr;
	ostr << "SELECT * FROM OrderState WHERE orderId='" << orderId << "' AND lastStateId='" << lastState << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Query Order state by id, " + string(mysql_error(m_mysqlConnection)));

	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetch_a_row();
	std::shared_ptr<OrderStateAbstractFactory> factory = nullptr;
	OrderStateParameters parameters;
	if(res.empty())
		throw QueryResultEmptyError("Query Order state by id and last state empty");
	else
	{
		double priceLow = toDouble(res[3]);
		double priceHigh = toDouble(res[4]);
		std::chrono::system_clock::time_point date = toTimePoint(res[0]);
		double transaction = toDouble(res[2]);
		unsigned long stateId = toUnsignedLong(res[6]);
		unsigned long lastStateId = toUnsignedLong(res[7]);
		parameters.date = date;
		parameters.range = AcceptableOrderPriceRange(priceLow, priceHigh);
		parameters.transactionPrice = transaction;
		parameters.currentStateId = stateId;
		parameters.lastStateId = lastStateId;

		string type = res[5];
		factory = findFactory(type);
	}

	return tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>(factory, parameters);
}

std::tuple<unsigned long, std::string, std::string, std::string> DatabaseConnection::checkPasswordAndGetUserInfo(
		std::string account, std::string password)
{
	// query
	ostringstream ostr;
	ostr << "SELECT * FROM user WHERE account='" << account << "' AND password='" << password << "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Check password, " + string(mysql_error(m_mysqlConnection)));

	// get result
	QueryResult result = mysql_store_result(m_mysqlConnection);
	auto res = result.fetch_a_row();
	unsigned long id;
	std::string name;
	std::string userPassword;
	std::string queryAccount;
	if(res.empty())
		throw PasswordNotRightError("Check password and get info empty");
	else
	{
		id = toUnsignedLong(res[0]);
		name = res[1];
		userPassword = res[2];
		queryAccount = res[3];
	}
	return tuple<unsigned long, std::string, std::string, std::string>(id, name, userPassword, queryAccount);
}

std::vector<std::tuple<>> DatabaseConnection::queryContactInfoByUserId(unsigned long userId)
{
	//TODO, read from Database
	return std::vector<std::tuple<>>();
}

void DatabaseConnection::updateUserPassword(std::string account, std::string password)
{
	//TODO:update user type
	
	// update password
	ostringstream ostr;
	ostr << "update user set password='" + password + "' where account='" + account + "'";
	string query = ostr.str();
	if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
		throw DatabaseInternalError("Update password, " + string(mysql_error(m_mysqlConnection)));
}

unsigned long DatabaseConnection::toUnsignedLong(std::string str)
{
	istringstream is(str);
	unsigned long ret{0};
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
