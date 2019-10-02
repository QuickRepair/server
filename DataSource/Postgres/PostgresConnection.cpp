 //
// Created by gaojian on 19-5-9.
//

#include "PostgresConnection.h"
#include "Errors/QueryResultEmptyError.hpp"
#include "Errors/AccountAlreadyExistError.hpp"
#include "Factories/OrderFactory.hpp"
#include "Order/OrderStates/OrderUnreceivedState.h"
#include "Order/OrderStates/OrderStartRepairState.h"
#include "Order/OrderStates/OrderRejectedState.h"
#include "Order/OrderStates/OrderReceivedState.h"
#include "Order/OrderStates/OrderFinishedState.h"
#include "Order/OrderStates/OrderEndRepairState.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include <chrono>
#include <ctime>
#include <experimental/array>

using std::make_unique;				using std::string;
using std::vector;					using std::tuple;
using std::make_tuple;				using std::to_string;
using std::list;					using std::shared_ptr;
using std::make_shared;				using std::chrono::system_clock;
using std::any;						using std::make_any;

PostgresConnection::PostgresConnection(std::string dbName, std::string userName, std::string password, std::string addr, unsigned port)
	: m_connection{"dbname="+dbName+" user="+userName+" password="+password+" hostaddr="+addr+" port="+to_string(port)},
	work{m_connection}
{}

std::string PostgresConnection::getPasswordFor(unsigned long id)
{
	pqxx::result result = work.exec("select password from quick_repair.account_schema.abstract_account where id=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["password"].as<std::string>();
}

void PostgresConnection::setPasswordFor(unsigned long id, const std::string &newPassword)
{
	work.exec("update quick_repair.account_schema.abstract_account set password='" + newPassword + "' where id=" + to_string(id) + ";");
	work.commit();
}

std::string PostgresConnection::getAccountFor(unsigned long id)
{
	pqxx::result result = work.exec("select account_number from quick_repair.account_schema.abstract_account where id=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["account_number"].as<std::string>();
}

void PostgresConnection::setAccountFor(unsigned long id, const std::string &newAccount)
{
	work.exec("update quick_repair.account_schema.abstract_account set account_number='" + newAccount + "' where id=" + to_string(id) + ";");
	work.commit();
}

std::string PostgresConnection::getOrderApplianceTypeFor(unsigned long id)
{
	pqxx::result result = work.exec("select appliance_type from quick_repair.order_schema.order where id=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["appliance_type"].as<std::string>();
}

std::string PostgresConnection::getOrderDetailFor(unsigned long id)
{
	pqxx::result result = work.exec("select detail_description from quick_repair.order_schema.order where id=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["appliance_type"].as<std::string>();
}

std::chrono::system_clock::time_point PostgresConnection::getOrderCreateDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='rejected';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

std::chrono::system_clock::time_point PostgresConnection::getOrderReceiveDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='received';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

std::chrono::system_clock::time_point PostgresConnection::getOrderStartRepairDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='start_repair';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

std::chrono::system_clock::time_point PostgresConnection::getOrderEndRepairDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='end_repair';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

std::chrono::system_clock::time_point PostgresConnection::getOrderFinishDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='finished';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

std::chrono::system_clock::time_point PostgresConnection::getOrderRejectDate(unsigned long id)
{
	pqxx::result result = work.exec("select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" + to_string(id) + " and state_type='unreceived';");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return system_clock::from_time_t(result[0]["date"].as<time_t>());
}

AcceptableOrderPriceRange PostgresConnection::getAcceptablePriceRange(unsigned long id)
{
	pqxx::result result = work.exec("select (price_range).lower, (price_range).higher from quick_repair.order_schema.order where id=" + to_string(id) + ";");
	work.commit();
	double lower, higher;
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	lower = result[0]["lower"].as<double>();
	higher = result[0]["higher"].as<double>();
	return AcceptableOrderPriceRange(lower, higher);
}

double PostgresConnection::getOrderTransaction(unsigned long id)
{
	pqxx::result result = work.exec("select transaction from quick_repair.order_schema.order where id=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["transaction"].as<double>();
}

void PostgresConnection::setOrderEvaluate(OrderEvaluate &evaluate)
{
	// TODO
}

OrderEvaluate PostgresConnection::getOrderEvaluate(unsigned long id)
{
	// TODO
}

std::list<std::string> PostgresConnection::getMerchantSupportedApplianceTypes(unsigned long id)
{
	pqxx::result result = work.exec("select appliance_type from quick_repair.account_details_schema.merchant_appliances where merchant=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	list<string> appliances;
	for (const auto &row : result)
		appliances.emplace_back(row["transaction"].as<string>());
	return appliances;
}

void PostgresConnection::updateSupportedAppliances(unsigned long id, std::list<std::string> types)
{
	string idStr = to_string(id);
	for (auto &type : types)
		pqxx::result result = work.exec("update quick_repair.account_details_schema.merchant_appliances set appliance_type='" + type + "' where merchant=" + idStr + ";");
	work.commit();
}

double PostgresConnection::getMaxSupportDistance(unsigned long id)
{
	pqxx::result result = work.exec("select max_support_distance from quick_repair.account_details_schema.merchant_address_info where merchant=" + to_string(id) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["max_support_distance"].as<double>();
}

void PostgresConnection::setMaxSupportDistance(unsigned long id, double distance)
{
	pqxx::result result = work.exec("update quick_repair.account_details_schema.merchant_address_info set max_support_distance=" + to_string(distance) + " where merchant=" + to_string(id) + ";");
	work.commit();
}

std::list<std::string> PostgresConnection::getAllowedService()
{
	pqxx::result result = work.exec("select type from quick_repair.qr_system.allowed_services;");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	list<string> appliances;
	for (const auto &row : result)
		appliances.emplace_back(row["type"].as<string>());
	return appliances;
}

unsigned long PostgresConnection::getOrderCommitter(unsigned long orderId)
{
	pqxx::result result = work.exec("select committer from quick_repair.order_schema.order where id=" + to_string(orderId) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["committer"].as<unsigned long>();
}

unsigned long PostgresConnection::getOrderReceiver(unsigned long orderId)
{
	pqxx::result result = work.exec("select acceptor from quick_repair.order_schema.order where id=" + to_string(orderId) + ";");
	work.commit();
	if(result.empty())
		throw QueryResultEmptyError("No such a result");
	return result[0]["acceptor"].as<unsigned long>();
}

unsigned long PostgresConnection::createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail)
{
	pqxx::result result = work.exec("insert into quick_repair.order_schema.order(committer, acceptor, appliance_type, detail_description, image, transaction, price_range, evaluate)\n"
									"values (" + to_string(committerId) + ", " +
									to_string(acceptorId) + ", '" +
									applianceType + "', '" +
									detail + "', null, null, (1, 2)::quick_repair.order_schema.acceptable_price_range, null) "
									"returning id;");
	unsigned long orderId = result[0][0].as<unsigned long>();
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
		   "VALUES (" + to_string(orderId) + ", now(), 'unreceived'::quick_repair.order_schema.order_state_enum);");
	work.commit();
	//work.exec("insert into order_state values(" + to_string(orderId) + ", array['unreceived'], now(), null, null, null, null, null, 0, 0, null, null);");
//	work.exec("update customer_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(committerId) + ";");
//	work.exec("update merchant_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(acceptorId) + ";");

	return orderId;
}

/*std::list<std::array<unsigned long, 3>>
PostgresConnection::getOrdersByAccountId(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select id, committer, acceptor "
									"from QuickRepair.OrderSchema.Order where committer=" + to_string(id) + " or acceptor=" + to_string(id) + ";");
	work.commit();

	std::list<std::array<unsigned long, 3>> orders;
	for(const auto &row : result)
	{
		orders.emplace_back(std::experimental::make_array(
				row["id"].as<unsigned long>(),
				row["committer"].as<unsigned long>(),
				row["acceptor"].as<unsigned long>()
				));
	}

	return orders;
}*/

std::list<std::any> PostgresConnection::getOrderStateByOrderId(unsigned long orderId)
{
	pqxx::result result = work.exec("select state_type "
								"from quick_repair.order_schema.order_state "
								"where order_id=" + to_string(orderId) + " order by state_change_date ASC;");
	work.commit();

	list<any> stateFactories;
	for(const auto &row : result)
	{
		string stateType = row["stateType"].as<string>();
#define MakeFactory(stateTypeName, FactoryTemplate)	\
	if(stateType == stateTypeName)					\
		stateFactories.emplace_back(make_any<shared_ptr<OrderStateFactory<FactoryTemplate>>>(make_shared<OrderStateFactory<FactoryTemplate>>()));

		MakeFactory("unreceived", OrderUnreceivedState)
		else MakeFactory("received", OrderReceivedState)
		else MakeFactory("start_repair", OrderStartRepairState)
		else MakeFactory("end_repair", OrderEndRepairState)
		else MakeFactory("finished", OrderFinishedState)
		else MakeFactory("rejected", OrderRejectedState)
	}

	return stateFactories;
}

unsigned long PostgresConnection::checkPasswordAndGetId(std::string account, std::string password)
{
	pqxx::result result = work.exec("select id from quick_repair.account_schema.abstract_account where account_number='" + account + "' and password='" + password + "';");
	work.commit();

	if(result.empty())
		throw QueryResultEmptyError("Can not find account or password wrong");
	return result[0]["id"].as<unsigned long>();
}

void PostgresConnection::createOrUpdatePasswordForMerchantAccount(std::string account, std::string password)
{
	pqxx::result result = work.exec("select id from quick_repair.account_schema.abstract_account where account_number='" + account + "' and type='merchant';");
	if(result.empty())		// create an account
		work.exec("insert into quick_repair.account_schema.merchant_account(account_number, password, name, type) "
			"VALUES(" + account + ", " + password + ", 'name', 'merchant'::quick_repair.account_schema.account_type);");
	else					// update password
	{
		unsigned long id = result[0]["id"].as<unsigned long>();
		work.exec("update quick_repair.account_schema.abstract_account set password=" + password + " where id=" + to_string(id) + ";");
	}
	work.commit();
}

void PostgresConnection::createOrUpdatePasswordForCustomerAccount(std::string account, std::string password)
{
	pqxx::result result = work.exec("select id from quick_repair.account_schema.abstract_account where account_number='" + account + "' and type='customer'");
	if(result.empty())		// create an account
		work.exec("insert into quick_repair.account_schema.customer_account(account_number, password, name, type) "
			"VALUES(" + account + ", " + password + ", 'name', 'customer'::quick_repair.account_schema.account_type);");
	else					// update password
	{
		unsigned long id = result[0]["id"].as<unsigned long>();
		work.exec("update quick_repair.account_schema.abstract_account set password=" + password + " where id=" + to_string(id) + ";");
	}
	work.commit();
}

/*std::vector<std::tuple<std::string, std::string>> PostgresConnection::queryContactInfoByUserId(unsigned long userId)
{
	pqxx::work work(*m_connection);
	// TODO
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
}*/

/*
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
*/

/*MerchantServiceType PostgresConnection::queryMerchantServiceType(unsigned long id)
{
	pqxx::work work(*m_connection);
	pqxx::result result = work.exec("select applianceType, maxSupportDistance "
								 "from QuickRepair.AccountDetailsSchema.MerchantAppliances, QuickRepair.AccountDetailsSchema.MerchantAddressInfo "
		 "where QuickRepair.AccountDetailsSchema.MerchantAppliances.id=" + to_string(id) + " and QuickRepair.AccountDetailsSchema.MerchantAddressInfo.id=" + to_string(id) + ";");
	work.commit();

	double max_repair_distance;
	list<string> appliance_types;
	if(result.empty())
		throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
	else
	{
		max_repair_distance = result[0]["maxSupportDistance"].as<double>();
		for (auto row : result)
		{
			if(result[0]["support_appliance_types"].is_null())
				throw QueryResultEmptyError("Find empty result for merchant(" + to_string(id) + ") service type");
			appliance_types.emplace_back(row["applianceType"].as<string>());
		}
	}
	return MerchantServiceType(appliance_types, max_repair_distance);
}*/

/*std::tuple<std::string, std::string> PostgresConnection::loadAccount(unsigned long id)
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
}*/

void PostgresConnection::updateMerchantServices(unsigned long id, std::list<std::string> types, double maxDistance)
{
	work.exec("update quick_repair.account_details_schema.merchant_address_info set max_support_distance=" + to_string(maxDistance) + " where merchant=" + to_string(maxDistance) + ";");
	for(auto &s : types)
		work.exec("update quick_repair.account_details_schema.merchant_appliances set appliance_type='" + s + "' where merchant=" + to_string(id) + ";");
	work.commit();
}

void PostgresConnection::orderReceived(unsigned long id)
{
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
								 "VALUES (" + to_string(id) + ", now(), 'received'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}

void PostgresConnection::orderRepairing(unsigned long id)
{
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
								 "VALUES (" + to_string(id) + ", now(), 'start_repair'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}

void PostgresConnection::orderEndRepair(unsigned long id, double price)
{
	work.exec("update quick_repair.order_schema.order set transaction=" + to_string(price) + " where id=" + to_string(price) + ";");
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
								 "VALUES (" + to_string(id) + ", now(), 'end_repair'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}

void PostgresConnection::orderPayed(unsigned long id)
{
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
		   "VALUES (" + to_string(id) + ", now(), 'finished'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}

void PostgresConnection::orderFinished(unsigned long id)
{
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
		   "VALUES (" + to_string(id) + ", now(), 'finished'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}

void PostgresConnection::orderRejected(unsigned long id)
{
	work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
		   "VALUES (" + to_string(id) + ", now(), 'reject'::quick_repair.order_schema.order_state_enum);");
	work.commit();
}
