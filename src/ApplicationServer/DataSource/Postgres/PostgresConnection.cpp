//
// Created by gaojian on 19-5-9.
//

#include "PostgresConnection.h"
#include "Errors/QueryResultEmptyError.hpp"
#include "Errors/AccountAlreadyExistError.hpp"
#include "Factories/OrderFactory.hpp"
#include "Order/OrderStates/OrderState.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include <chrono>
#include <ctime>
#include <iostream>

using std::make_unique;                using std::string;
using std::vector;                    using std::tuple;
using std::make_tuple;                using std::to_string;
using std::list;                    using std::shared_ptr;
using std::make_shared;                using std::chrono::system_clock;
using std::any;                        using std::make_any;
using std::cerr;                    using std::endl;
using std::runtime_error;

PostgresConnection::PostgresConnection(std::string dbName, std::string userName, std::string password, std::string addr,
									   unsigned port)
		: m_connection{
		"dbname=" + dbName + " user=" + userName + " password=" + password + " hostaddr=" + addr + " port=" +
		to_string(port)}
{}

std::string PostgresConnection::getPasswordFor(AccountId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select password from quick_repair.account_schema.abstract_account where id=" + to_string(id) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["password"].as<std::string>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::setPasswordFor(AccountId id, const std::string &newPassword)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("update quick_repair.account_schema.abstract_account set password='" + newPassword + "' where id=" +
				  to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::string PostgresConnection::getAccountFor(AccountId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select account_number from quick_repair.account_schema.abstract_account where id=" + to_string(id) +
				";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["account_number"].as<std::string>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::setAccountFor(AccountId id, const std::string &newAccount)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("update quick_repair.account_schema.abstract_account set account_number='" + newAccount +
				  "' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::string PostgresConnection::getOrderApplianceTypeFor(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select appliance_type from quick_repair.order_schema.order_item where id=" + to_string(id) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["appliance_type"].as<std::string>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::string PostgresConnection::getOrderDetailFor(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select detail_description from quick_repair.order_schema.order_item where id=" + to_string(id) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["detail_description"].as<std::string>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderCreateDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='unreceived';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderReceiveDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='received';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderStartRepairDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='start_repair';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderEndRepairDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='end_repair';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderFinishDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='finished';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::chrono::system_clock::time_point PostgresConnection::getOrderRejectDate(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select extract(epoch from state_change_date)::bigint as date from quick_repair.order_schema.order_state where order_id=" +
				to_string(id) + " and state_type='rejected';"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return system_clock::from_time_t(result[0]["date"].as<time_t>());
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

AcceptableOrderPriceRange PostgresConnection::getAcceptablePriceRange(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select (price_range).lower, (price_range).higher from quick_repair.order_schema.order_item where id=" +
				to_string(id) + ";"
		);
		work.commit();
		double lower, higher;
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		lower = result[0]["lower"].as<double>();
		higher = result[0]["higher"].as<double>();
		return AcceptableOrderPriceRange(lower, higher);
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}

}

double PostgresConnection::getOrderTransaction(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select transaction from quick_repair.order_schema.order_item where id=" + to_string(id) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["transaction"].as<double>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::setOrderEvaluate(OrderEvaluate evaluate)
{
	// TODO
}

OrderEvaluate PostgresConnection::getOrderEvaluate(OrderId id)
{
	// TODO
}

std::list<std::string> PostgresConnection::getMerchantSupportedApplianceTypes(AccountId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select appliance_type from quick_repair.account_details_schema.merchant_appliances where merchant=" +
				to_string(id) + ";"
		);
		work.commit();
		list<string> appliances;
		if (!result.empty())
			for (const auto &row : result)
				appliances.emplace_back(row["appliance_type"].as<string>());
		return appliances;
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::updateSupportedAppliances(AccountId id, std::list<std::string> types)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec(
				"delete from quick_repair.account_details_schema.merchant_appliances where merchant=" + to_string(id) +
				";"
		);
		for (auto &s : types)
			work.exec(
					"insert into quick_repair.account_details_schema.merchant_appliances(merchant, appliance_type) values (" +
					to_string(id) + ", '" + s + "';"
			);
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

double PostgresConnection::getMaxSupportDistance(AccountId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select max_support_distance from quick_repair.account_details_schema.merchant_address_info where merchant=" +
				to_string(id) + ";"
		);
		work.commit();
		double distance = -1;
		if (!result.empty())
			distance = result[0]["max_support_distance"].as<double>();
		return distance;
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::setMaxSupportDistance(AccountId id, double distance)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"update quick_repair.account_details_schema.merchant_address_info set max_support_distance=" +
				to_string(distance) + " where merchant=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::list<OrderId> PostgresConnection::getOrderList(AccountId id)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select id from quick_repair.order_schema.order_item where committer=" + to_string(id) + " or acceptor=" +
				to_string(id) + ";"
		);
		work.commit();
		list<OrderId> orders;
		for (const auto &row : result)
			orders.emplace_back(row["id"].as<unsigned long>());
		return orders;
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

std::list<std::string> PostgresConnection::getAllowedService()
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec("select type from quick_repair.qr_system.allowed_services;");
		work.commit();
		list<string> appliances;
		for (const auto &row : result)
			appliances.emplace_back(row["type"].as<string>());
		return appliances;
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

AccountId PostgresConnection::getOrderCommitter(OrderId orderId)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select committer from quick_repair.order_schema.order_item where id=" + to_string(orderId) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["committer"].as<unsigned long>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

AccountId PostgresConnection::getOrderReceiver(OrderId orderId)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select acceptor from quick_repair.order_schema.order_item where id=" + to_string(orderId) + ";"
		);
		work.commit();
		if (result.empty())
			throw QueryResultEmptyError("No such a result");
		return result[0]["acceptor"].as<unsigned long>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

OrderId PostgresConnection::createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType,
										std::string detail)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"insert into quick_repair.order_schema.order_item(committer, acceptor, appliance_type, detail_description, image, transaction, price_range, evaluate) "
				"values (" + to_string(committerId) + ", " +
				to_string(acceptorId) + ", '" +
				applianceType + "', '" +
				detail + "', null, null, (1, 2)::quick_repair.order_schema.acceptable_price_range, null) "
						 "returning id;"
		);
		OrderId orderId = result[0][0].as<unsigned long>();
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(orderId) +
				  ", now(), 'unreceived'::quick_repair.order_schema.order_state_enum);"
		);
		work.commit();
		//work.exec("insert into order_state values(" + to_string(orderId) + ", array['unreceived'], now(), null, null, null, null, null, 0, 0, null, null);");
//	work.exec("update customer_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(committerId) + ";");
//	work.exec("update merchant_account set orders_id=array_append(orders_id, " + to_string(orderId) + ") where id=" + to_string(acceptorId) + ";");

		return orderId;
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
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

std::list<std::any> PostgresConnection::getOrderStateByOrderId(OrderId orderId)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec("select state_type "
										"from quick_repair.order_schema.order_state "
										"where order_id=" + to_string(orderId) + " order by state_change_date ASC;"
		);
		work.commit();

		list<any> stateFactories;
		for (const auto &row : result)
		{
			string stateType = row["state_type"].as<string>();
#define MakeFactory(stateTypeName, FactoryTemplate)    \
    if(stateType == stateTypeName)                    \
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
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

AccountId PostgresConnection::checkPasswordAndGetId(std::string account, std::string password)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select id from quick_repair.account_schema.abstract_account where account_number='" + account +
				"' and password='" + password + "';"
		);
		work.commit();

		if (result.empty())
			throw QueryResultEmptyError("Can not find account or password wrong");
		return result[0]["id"].as<unsigned long>();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::createOrUpdatePasswordForMerchantAccount(std::string account, std::string password)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select id from quick_repair.account_schema.abstract_account where account_number='" + account +
				"' and type='merchant';"
		);
		if (result.empty())        // create an account
		{
			pqxx::result r = work.exec(
					"insert into quick_repair.account_schema.merchant_account(account_number, password, name, type) "
					"VALUES(" + account + ", " + password +
					", 'name', 'merchant'::quick_repair.account_schema.account_type) returning id;"
			);
			AccountId id = r[0][0].as<unsigned long>();
			work.exec(
					"insert into quick_repair.account_details_schema.merchant_address_info(merchant, address, max_support_distance, longitude, latitude) VALUES (" +
					to_string(id) + ", '', 0, -1, -1);"
			);
		} else                    // update password
		{
			unsigned long id = result[0]["id"].as<unsigned long>();
			work.exec("update quick_repair.account_schema.abstract_account set password=" + password + " where id=" +
					  to_string(id) + ";"
			);
		}
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
}

void PostgresConnection::createOrUpdatePasswordForCustomerAccount(std::string account, std::string password)
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec(
				"select id from quick_repair.account_schema.abstract_account where account_number='" + account +
				"' and type='customer'"
		);
		if (result.empty())        // create an account
			work.exec("insert into quick_repair.account_schema.customer_account(account_number, password, name, type) "
					  "VALUES(" + account + ", " + password +
					  ", 'name', 'customer'::quick_repair.account_schema.account_type);"
			);
		else                    // update password
		{
			unsigned long id = result[0]["id"].as<unsigned long>();
			work.exec("update quick_repair.account_schema.abstract_account set password=" + password + " where id=" +
					  to_string(id) + ";"
			);
		}
		work.commit();
	}
	catch (runtime_error &e)
	{
		cerr << e.what() << endl;
	}
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

void PostgresConnection::updateMerchantServices(AccountId id, std::list<std::string> types, double maxDistance)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("update quick_repair.account_details_schema.merchant_address_info set max_support_distance=" +
				  to_string(maxDistance) + " where merchant=" + to_string(id) + ";"
		);
		work.exec(
				"delete from quick_repair.account_details_schema.merchant_appliances where merchant=" + to_string(id) +
				";"
		);
		for (auto &s : types)
			work.exec(
					"insert into quick_repair.account_details_schema.merchant_appliances(merchant, appliance_type) values (" +
					to_string(id) + ", '" + s + "');"
			);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderReceived(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'received'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set current_state='received' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderRepairing(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'start_repair'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set current_state='start_repair' where id=" + to_string(id) +
				  ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderEndRepair(OrderId id, double price)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'end_repair'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set transaction=" + to_string(price) + " where id=" +
				  to_string(price) + ";"
		);
		work.exec(
				"update quick_repair.order_schema.order_item set current_state='end_repair' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderPayed(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'finished'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set current_state='finished' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderFinished(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'finished'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set current_state='finished' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void PostgresConnection::orderRejected(OrderId id)
{
	try
	{
		pqxx::work work{m_connection};
		work.exec("insert into quick_repair.order_schema.order_state(order_id, state_change_date, state_type) "
				  "VALUES (" + to_string(id) + ", now(), 'rejected'::quick_repair.order_schema.order_state_enum);"
		);
		work.exec("update quick_repair.order_schema.order_item set current_state='rejected' where id=" + to_string(id) + ";"
		);
		work.commit();
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

std::list<AccountId> PostgresConnection::getAllMerchants()
{
	try
	{
		pqxx::work work{m_connection};
		pqxx::result result = work.exec("select id from quick_repair.account_schema.merchant_account;");
		work.commit();
		list<AccountId> merchants;
		for (const auto &row : result)
			merchants.emplace_back(row["id"].as<unsigned long>());
		return merchants;
	}
	catch (runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
