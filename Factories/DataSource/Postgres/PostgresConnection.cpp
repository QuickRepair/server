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
	pqxx::result result = work.exec("insert into orders(committer, acceptor, appliance_type, detail_description)"
									"values(" + to_string(committerId) + ", "
									"       " + to_string(acceptorId) + ", "
									"       '" + applianceType + "', "
									"       '" + detail + "'"
									") returning id;"
									);
	unsigned long orderId;
	if(result.empty())
		throw QueryResultEmptyError("Can not create id for new order");
	else
		orderId = result[0][0].as<unsigned long>();

	//work.exec("insert into order_state values(" + to_string(orderId) + ", array['unreceived'], now(), null, null, null, null, null, 0, 0, null, null);");
	work.exec("insert into order_state values(" + to_string(orderId) + ", array[('unreceived')::state_enums], now(), null, null, null, null, null, 0, 0, null, null);");
	work.exec("update customer_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(committerId) + ";");
	work.exec("update merchant_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(acceptorId) + ";");
	work.commit();

	return orderId;
}

std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string>>
PostgresConnection::queryOrderByAccountId(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select id, committer, acceptor, appliance_type, detail_description "
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

std::tuple<std::vector<std::shared_ptr<OrderStateAbstractFactory>>, OrderStateParameters>
PostgresConnection::queryOrderStateByOrderId(unsigned long orderId)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select state_type,"
									"       extract(epoch from create_date)::bigint as create_date, "
									"       extract(epoch from received_date)::bigint as received_date, "
									"       extract(epoch from start_repair_date)::bigint as start_repair_date, "
									"       extract(epoch from end_repair_date)::bigint as end_repair_date, "
									"       extract(epoch from finish_date)::bigint as finish_date, "
									"       extract(epoch from reject_date)::bigint as reject_date, "
									"       price_range_lower, price_range_higher, transaction_price, evaluate_text "
									"from order_state "
									"where order_id=" + to_string(orderId) + ";");
	work.commit();

	tuple<vector<shared_ptr<OrderStateAbstractFactory>>, OrderStateParameters> statesOfOneOrder;
	vector<shared_ptr<OrderStateAbstractFactory>> stateFactories;
	OrderStateParameters stateParameters;
	for(const auto &row : result)
	{
		shared_ptr<OrderStateAbstractFactory> factory;
		auto stateTypesArray = row["state_type"].as_array();
		for(auto nextType = stateTypesArray.get_next(); nextType.first != pqxx::array_parser::juncture::done; nextType = stateTypesArray.get_next())
			if(nextType.first == pqxx::array_parser::juncture::string_value)
			{
				if(nextType.second == "unreceived")
				{
					stateParameters.create_date = system_clock::from_time_t(row["create_date"].as<time_t>());
					stateParameters.range = AcceptableOrderPriceRange(row["price_range_lower"].as<double>(), row["price_range_higher"].as<double>());
					factory = make_shared<OrderUnreceivedStateFactory>();
				}
				else if(nextType.second == "received")
				{
					stateParameters.received_date = system_clock::from_time_t(row["received_date"].as<time_t>());
					factory = make_shared<OrderReceivedStateFactory>();
				}
				else if(nextType.second == "start_repair")
				{
					stateParameters.start_repair_date = system_clock::from_time_t(row["start_repair_date"].as<time_t>());
					factory = make_shared<OrderStartRepairStateFactory>();
				}
				else if(nextType.second == "end_repair")
				{
					stateParameters.end_repair_date = system_clock::from_time_t(row["end_repair_date"].as<time_t>());
					stateParameters.transactionPrice = row["transaction_price"].as<double>();
					factory = make_shared<OrderEndRepairStateFactory>();
				}
				else if(nextType.second == "finished")
				{
					stateParameters.finish_date = system_clock::from_time_t(row["finish_date"].as<time_t>());
					//TODO stateParameters.evaluate = OrderEvaluate(row["evaluate_text"].as<string>());
					factory = make_shared<OrderFinishedStateFactory>();
				}
				else if(nextType.second == "rejected")
				{
					stateParameters.reject_date = system_clock::from_time_t(row["reject_date"].as<time_t>());
					factory = make_shared<OrderRejectedStateFactory>();
				}
				stateFactories.emplace_back(factory);
			}
	}

	return make_tuple(stateFactories, stateParameters);
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
		if(result[0]["support_appliance_types"].is_null())
			throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
		auto supportedTypeArray = result[0]["support_appliance_types"].as_array();
		for(auto nextType = supportedTypeArray.get_next(); nextType.first != pqxx::array_parser::juncture::done; nextType = supportedTypeArray.get_next())
			if(nextType.first == pqxx::array_parser::juncture::string_value)
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

void PostgresConnection::updateSupportedServices(unsigned long id, std::list<std::string> types, double maxDistance)
{
	string typesList;
	for(auto &s : types)
		typesList += "'" + s + "',";
	typesList.back() = ' ';
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update merchant_account set "
								 "max_repair_distance=" + to_string(maxDistance) + ", support_appliance_types=array[" + typesList + "] "
														   "where id=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderReceived(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update order_state "
									"set state_type=state_type||('received')::state_enums, received_date=now() "
									"where order_id=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderRepairing(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update order_state "
									"set state_type=state_type||('start_repair')::state_enums, start_repair_date=now() "
									"where order_id=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderEndRepair(unsigned long id, double price)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update order_state "
									"set state_type=state_type||('end_repair')::state_enums, end_repair_date=now(), transaction_price=" + to_string(price) + " "
									"where order_id=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderFinished(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update order_state "
									"set state_type=state_type||('finished')::state_enums, finish_date=now() "
									"where order_id=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderRejected(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("update order_state "
									"set state_type=state_type||('rejected')::state_enums, reject_date=now() "
									"where order_id=" + to_string(id) + ";");
	work.commit();
}