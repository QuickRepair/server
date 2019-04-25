//
// Created by gaojian on 19-4-13.
//

#include "SimulateDatabase.h"
#include "Factories/DataSource/OrderStateParameters.h"
#include "Errors/PasswordNotRightError.hpp"
#include "Errors/QueryResultEmptyError.hpp"
#include "Errors/AccountAlreadyExistError.hpp"
#include "Errors/NoSuchAnAccountError.hpp"
#include "Factories/OrderStateFactories/OrderUnreceivedStateFactory.h"
#include "Factories/OrderStateFactories/OrderReceivedStateFactory.h"
#include "Factories/OrderStateFactories/OrderEndRepairStateFactory.h"
#include "Factories/OrderStateFactories/OrderFinishedStateFactory.h"
#include "Factories/OrderStateFactories/OrderStartRepairStateFactory.h"

using std::get;									using std::make_tuple;
using std::vector;								using std::tuple;
using std::string;								using std::list;
using std::chrono::system_clock;				using std::chrono::system_clock;
using std::shared_ptr;							using std::make_shared;

SimulateDatabase::SimulateDatabase()
{
	/* table account in database should contain following rows
	 * id	account	password	type
	 * 6	1234	1234		merchant
	 * 9	1		1			customer
	 */
	m_account.push_back(
			make_tuple<unsigned long, string, string, string, unsigned long>(
					6, "1234", "1234", "merchant", 1
			)
	);
	m_account.push_back(
			make_tuple<unsigned long, string, string, string, unsigned long>(
					9, "1", "1", "customer", -1
			)
	);

	/* table orders in database should contain following rows
	 * id	committer	accpetor	appliance_type	detail	current_state
	 * 34	9			6			type			detail	3
	 */
	m_orders.push_back(
			make_tuple<unsigned long, unsigned long, unsigned long, string, string, unsigned long>(
					34, 9, 6, "type", "detail", 3
			)
	);

	/* table order_states should contain following rows
	 * order_id	id	last_state	date	transaction	state_name
	 * 34		3	null		now()	null		unreceived
	 */
	m_orderStates.push_back(
			make_tuple<unsigned long, int, unsigned long, system_clock::time_point, double, string>(
					34, 3, -1, system_clock::now(), -1, "unreceived"
			)
	);
}

unsigned long SimulateDatabase::createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail)
{
	auto newOrder = make_tuple<unsigned long, unsigned long, unsigned long, string, string, unsigned long>((get<0>(m_orders.back()) + 1), std::move(committerId), std::move(acceptorId), std::move(applianceType), std::move(detail), 0);
	auto newOrderState = make_tuple<unsigned long, unsigned long, unsigned long, system_clock::time_point, double, string>((get<0>(m_orders.back()) + 1), std::move(get<1>(m_orderStates.back()) + 1), -1, std::move(system_clock::now()), -1, string("unreceived"));
	m_orders.push_back(newOrder);
	m_orderStates.emplace_back(newOrderState);
	return get<0>(m_orders.back()) + 1;
}

std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> SimulateDatabase::queryOrderByAccountId(unsigned long id)
{
	list<tuple<unsigned long, unsigned long, unsigned long, string, string, unsigned long>> orders;
	for(auto &t : m_orders)
	{
		if(get<1>(t) == id || get<2>(t) == id)
			orders.push_back(t);
	}
	return orders;
}

std::vector<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
SimulateDatabase::queryOrderStateByOrderId(unsigned long orderId)
{
	vector<tuple<shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>> states;

	for(auto &t : m_orders)
	{
		// find the order
		if(get<0>(t) == orderId)
		{
			// get last state
			int lastState = get<5>(t);
			// get all state
			while(lastState != -1)
			{
				for(auto &state : m_orderStates)
					if(get<0>(state) == orderId && get<1>(state) == lastState)
					{
						string stateType = get<5>(state);
						OrderStateParameters parameters;

						parameters.date = get<3>(state);
						parameters.range = AcceptableOrderPriceRange();
						parameters.transactionPrice = get<4>(state);
						parameters.currentStateId = get<1>(state);
						parameters.lastStateId = get<2>(state);
						states.push_back(
								make_tuple<shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>(
										std::move(findFactory(stateType)), std::move(parameters)
								)
						);
						// update last state
						lastState = get<2>(state);
					}
			}
			break;
		}
	}
	return states;
}

unsigned long SimulateDatabase::checkMerchantPasswordAndGetId(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account)
		{
			if(get<2>(t) == password && get<3>(t) == "merchant")
				return get<0>(t);
			else
				throw PasswordNotRightError("Check merchant password and get id");
		}
	}
	throw NoSuchAnAccountError("Check merchant password and get id");
}

unsigned long SimulateDatabase::checkCustomerPasswordAndGetId(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account)
		{
			if(get<2>(t) == password && get<3>(t) == "customer")
				return get<0>(t);
			else
				throw PasswordNotRightError("Check customer password and get id");
		}
	}
	throw NoSuchAnAccountError("Check customer password and get id");
}

std::vector<std::tuple<>> SimulateDatabase::queryContactInfoByUserId(unsigned long userId)
{
	//TODO
	return vector<tuple<>>();
}

void SimulateDatabase::createMerchantAccount(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account)
			throw AccountAlreadyExistError("Create merchant account");
	}
	auto merchant = make_tuple<unsigned long, string, string, string, unsigned long>((get<0>(m_account.back()) + 1), std::move(account), std::move(password), "merchant", 1);
	m_account.push_back(merchant);
}

void SimulateDatabase::createCustomerAccount(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account)
			throw AccountAlreadyExistError("Create customer account");
	}
	auto customer = make_tuple<unsigned long, string, string, string, unsigned long>((get<0>(m_account.back()) + 1), std::move(account), std::move(password), string("customer"), 1);
	m_account.push_back(customer);
}

void SimulateDatabase::updateMerchantAccountPassword(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account && get<3>(t) == "merchant")
			get<2>(t) = password;
	}
	throw NoSuchAnAccountError("Update merchant account password");
}

void SimulateDatabase::updateCustomerAccountPassword(std::string account, std::string password)
{
	for(auto &t : m_account)
	{
		if(get<1>(t) == account && get<3>(t) == "customer")
			get<2>(t) = password;
	}
	throw NoSuchAnAccountError("Update customer account password");
}

std::tuple<std::list<std::string>, int> SimulateDatabase::queryMerchantServiceType(unsigned long id)
{
	list<string> types;
	for(auto &t : m_serviceType)
		if(get<1>(t) == id)
			types = get<0>(t);

	unsigned long max_service_distance;
	for(auto &t : m_account)
		if(get<0>(t) == id)
			max_service_distance = get<4>(t);
	
	max_service_distance = static_cast<int>(max_service_distance);
	return make_tuple<list<string>, int>(std::move(types), max_service_distance);
}

std::tuple<std::string, std::string> SimulateDatabase::loadMerchant(unsigned long id)
{
	for(auto &t : m_account)
	{
		if(get<0>(t) == id && get<3>(t) == "merchant")
		{
			string account = get<1>(t);
			string password = get<2>(t);
			return make_tuple<string, string>(std::move(account), std::move(password));
		}
	}
	throw QueryResultEmptyError("Load merchant");
}

std::tuple<std::string, std::string> SimulateDatabase::loadCustomer(unsigned long id)
{
	for(auto &t : m_account)
	{
		if(get<0>(t) == id && get<3>(t) == "customer")
		{
			string account = get<1>(t);
			string password = get<2>(t);
			return make_tuple<string, string>(std::move(account), std::move(password));
		}
	}
	throw QueryResultEmptyError("Load customer");
}

std::shared_ptr<OrderStateAbstractFactory> SimulateDatabase::findFactory(std::string orderType)
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
