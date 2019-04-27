#include "OrderFactory.h"
#include "OrderStateFactories/OrderStateAbstractFactory.h"
#include "Factories/DataSource/MariaDB/DatabaseConnection.h"
#include "Factories/DataSource/SimulateDatabase/SimulateDatabase.h"
#include "Factories/DataSource/OrderStateParameters.h"
#include "Order/Order.h"
#include "Order/OrderStates/OrderState.h"
#include "Account/ContactInformation.h"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Errors/QueryResultEmptyError.hpp"
#include <tuple>

using std::make_shared;			using std::get;
using std::shared_ptr;			using std::vector;
using std::tuple;				using std::list;

std::list<std::shared_ptr<Order>> OrderFactory::getOrdersListForAccount(std::weak_ptr<Account> account)
{
	list<shared_ptr<Order>> orderList;
	try
	{
		auto orderInfo = DATA_SOURCE_FROM::getInstance().queryOrderByAccountId(account.lock()->id());
		for (auto &ret : orderInfo)
		{
			//TODO: load contact information
			ContactInformation tmp;
			shared_ptr<Order> newOrder = make_shared<Order>(get<0>(ret), get<1>(ret), get<2>(ret), get<3>(ret), tmp, get<4>(ret));
			newOrder->m_currentState = getStates(newOrder);

			orderList.push_back(newOrder);
		}
	}
	catch (QueryResultEmptyError &e)
	{
	}
	return orderList;
}

std::shared_ptr<Order> OrderFactory::createOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
												 std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range)
{
	unsigned long id = DATA_SOURCE_FROM::getInstance().createOrder(committer.lock()->id(), acceptor.lock()->id(), applianceType, detail);
	shared_ptr<Order> newOrder = make_shared<Order>(id, committer, acceptor, applianceType, contactWay, detail);
	newOrder->initOrderState(range);
	return newOrder;
}

std::shared_ptr<OrderState> OrderFactory::getStates(shared_ptr<Order> &order)
{
	try
	{
		vector<tuple<shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>> stateInfo;
		stateInfo = DATA_SOURCE_FROM::getInstance().queryOrderStateByOrderId(order->id());

		shared_ptr<OrderStateAbstractFactory> stateFactory;
		OrderStateParameters parameters;

		for(auto state = stateInfo.rbegin(); state != stateInfo.rend(); ++state)
		{
			stateFactory = get<0>(*state);
			parameters = get<1>(*state);
			parameters.lastState = stateFactory->makeStateForOrder(order, parameters);
		}

		return parameters.lastState;
	}
	catch (QueryResultEmptyError &e)
	{
		return nullptr;
	}
}
