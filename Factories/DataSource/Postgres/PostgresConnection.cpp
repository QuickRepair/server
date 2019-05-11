//
// Created by gaojian on 19-5-9.
//

#include "Errors/QueryResultEmptyError.hpp"
#include "PostgresConnection.h"
#include "Configuration/Configure.h"
#include "Factories/DataSource/OrderStateParameters.h"
#include "Factories/OrderStateFactories/OrderStateAbstractFactory.h"
#include "Factories/OrderStateFactories/OrderUnreceivedStateFactory.h"
#include "Factories/OrderStateFactories/OrderReceivedStateFactory.h"
#include "Factories/OrderStateFactories/OrderStartRepairStateFactory.h"
#include "Factories/OrderStateFactories/OrderEndRepairStateFactory.h"
#include "Factories/OrderStateFactories/OrderFinishedStateFactory.h"
#include "Factories/OrderStateFactories/OrderRejectedStateFactory.h"
#include "Errors/NoSuchAnAccountOrPasswordNotRightError.hpp"
#include "Errors/AccountAlreadyExistError.hpp"
#include <chrono>
#include <ctime>

using std::make_unique;				using std::string;
using std::vector;					using std::tuple;
using std::make_tuple;				using std::to_string;
using std::list;					using std::shared_ptr;
using std::make_shared;				using std::chrono::system_clock;

PostgresConnection::PostgresConnection()
{
	Configure configure;
	string connectConfig = "dbname=" + configure.databaseName()
			+ " user=" + configure.databaseUserName()
			+ " password=" + configure.databasePassword()
			+ " hostaddr=" + configure.databaseIp()
			+ " port=" + to_string(configure.databasePort());
	m_connection = make_unique<pqxx::connection>(connectConfig);
}

unsigned long PostgresConnection::createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("insert into orders(committer, acceptor, appliance_type, detail_description, order_state_history)"
									"values(" + to_string(committerId) + ", "
									"       " + to_string(acceptorId) + ", "
									"       '" + applianceType + "', "
									"       '" + detail + "', "
									"       array['unreceived', now(), 0, 0, null, null])"
									"returning id;");

	unsigned long orderId;
	if(result.empty())
		throw QueryResultEmptyError("Can not create id for new order");
	else
		orderId = result[0][0].as<unsigned long>();

	work.exec("update customer_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(committerId) + ";");
	work.exec("update merchant_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(committerId) + ";");
	work.commit();

	return orderId;
}

std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string>>
PostgresConnection::queryOrderByAccountId(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select id, committer, acceptor, appliance_type, detail_description"
									"from orders where id in (select unnest(orders_id) from customer_account where id=" + to_string(id) + ");");
	work.commit();

	list<tuple<unsigned long, unsigned long, unsigned long, string, string>> orders;
	for(const auto &row : result)
	{
		orders.emplace_back(make_tuple(
				row["id"].as<unsigned long>(),
				row["committer"].as<unsigned long>(),
				row["acceptor"].as<unsigned long>(),
				row["appliance_type"].as<string>(),
				row["detail_description"].as<string>()
				));
	}

	return orders;
}

std::vector<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
PostgresConnection::queryOrderStateByOrderId(unsigned long orderId)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select state_type, state_change_date, price_range_lower, price_range_higher, transaction_price, evaluate_text"
									"from order_state where order_id=" + to_string(orderId) + " order by state_id desc;");
	work.commit();

	vector<tuple<shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>> statesOfOneOrder;
	string stateType;
	for(const auto &row : result)
	{
		OrderStateParameters stateParameters;
		shared_ptr<OrderStateAbstractFactory> factory;
		stateType = row["state_type"].as<string>();
		if(stateType == "unreceived")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			stateParameters.range = AcceptableOrderPriceRange(row["price_range_lower"].as<double>(), row["price_range_higher"].as<double>());
			factory = make_shared<OrderUnreceivedStateFactory>();
		}
		else if(stateType == "received")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			factory = make_shared<OrderReceivedStateFactory>();
		}
		else if(stateType == "startRepair")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			factory = make_shared<OrderStartRepairStateFactory>();
		}
		else if(stateType == "endRepair")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			stateParameters.transactionPrice = row["transaction_price"].as<double>();
			factory = make_shared<OrderEndRepairStateFactory>();
		}
		else if(stateType == "finished")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			//TODO stateParameters.evaluate = OrderEvaluate(row["evaluate_text"].as<string>());
			factory = make_shared<OrderFinishedStateFactory>();
		}
		else if(stateType == "rejected")
		{
			stateParameters.date = system_clock::from_time_t(row["state_change_date"].as<time_t>());
			factory = make_shared<OrderRejectedStateFactory>();
		}
		statesOfOneOrder.emplace_back(make_tuple(factory, stateParameters));
	}

	return statesOfOneOrder;
}

unsigned long PostgresConnection::checkPasswordAndGetId(std::string account, std::string password)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select id from account where account_name='" + account + "' and password='" + password + "';");
	work.commit();

	unsigned long id;
	if(result.empty())
		throw NoSuchAnAccountOrPasswordNotRightError("Can not find account or password wrong");
	else
		id = result[0]["id"].as<unsigned long>();

	return id;
}

std::vector<std::tuple<std::string, std::string>> PostgresConnection::queryContactInfoByUserId(unsigned long userId)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select province, city, detail_address, telephone from contact_information where account_id=" + to_string(userId) + ";");
	work.commit();

	vector<tuple<string, string>> ret;
	string address;
	string tel;
	if(result.empty())
		throw QueryResultEmptyError("Can not find account or password wrong");
	else
	{
		for(const auto &row : result)
		{
			address = row["province"].as<string>() + row["city"].as<string>() + row["detail_address"].as<string>();
			tel = row["telephone"].as<string>();
			ret.emplace_back(make_tuple(address, tel));
		}
	}
	return ret;
}

void PostgresConnection::createMerchantAccount(std::string account, std::string password)
{
	try
	{
		pqxx::work work(*m_connection);
		pqxx::result result = work.exec(
				"insert into merchant_account(account_name, password) values('" + account + "', '" + password + "');"
		);
		work.commit();
	}
	catch(pqxx::sql_error &e)
	{
		throw AccountAlreadyExistError("Account for " + account + " already exist");
	}
}

void PostgresConnection::createCustomerAccount(std::string account, std::string password)
{
	try
	{
		pqxx::work work(*m_connection);
		pqxx::result result = work.exec("insert into customer_account(account_name, password) values('" + account + "', '" + password + "');");
		work.commit();
	}
	catch(pqxx::sql_error &e)
	{
		throw AccountAlreadyExistError("Account for " + account + " already exist");
	}
}

void PostgresConnection::updateAccountPassword(std::string account, std::string newPassword)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update account set password='" + newPassword + "' where account_name='" + account + "';");
	work.commit();
}

std::tuple<std::list<std::string>, double> PostgresConnection::queryMerchantServiceType(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select max_repair_distance, support_appliance_types from merchant_account where id=" + to_string(id) + ";");
	work.commit();

	double max_repair_distance;
	list<string> appliance_types;
	if(result.empty())
		throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
	else
	{
		if(result[0]["max_repair_distance"].is_null())
			throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
		max_repair_distance = result[0]["max_repair_distance"].as<double>();
		if(result[0]["appliance_types"].is_null())
			throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
		auto supportedTypeArray = result[0]["appliance_types"].as_array();
		for(auto nextType = supportedTypeArray.get_next(); nextType.first == pqxx::array_parser::juncture::string_value; nextType = supportedTypeArray.get_next())
			appliance_types.emplace_back(nextType.second);
	}
	return make_tuple(appliance_types, max_repair_distance);
}

std::tuple<std::string, std::string> PostgresConnection::loadAccount(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select account_name, password from account where id=" + to_string(id) + ";");
	work.commit();

	string account;
	string password;
	if(result.empty())
		throw QueryResultEmptyError("Not find account(" + to_string(id) + ")");
	else
	{
		account = result[0]["account_name"].as<string>();
		password = result[0]["password"].as<string>();
	}
	return make_tuple(account, password);
}
