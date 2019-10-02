#include "OrderFactory.hpp"
#include "DataSource/DataSource.hpp"
#include "Order/Order.h"
#include <list>
#include <any>
#include "Order/OrderStates/OrderUnreceivedState.h"
#include "Order/OrderStates/OrderStartRepairState.h"
#include "Order/OrderStates/OrderRejectedState.h"
#include "Order/OrderStates/OrderReceivedState.h"
#include "Order/OrderStates/OrderFinishedState.h"
#include "Order/OrderStates/OrderEndRepairState.h"
#include "ManagerProxy/AccountManagerProxy.h"
/*#include "Order/OrderStates/OrderState.h"
#include "Account/ContactInformation.h"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Errors/QueryResultEmptyError.hpp"
#include <tuple>*/

using std::make_shared;			using std::shared_ptr;
using std::unique_ptr;			using std::list;
using std::any;

/*std::list<std::shared_ptr<Order>> OrderFactory::getOrdersListForAccount(std::weak_ptr<Account> account)
{
	AccountManagerProxy accountManager;
	list<shared_ptr<Order>> orderList;
	try
	{
		auto orderInfo = DataSource::getDataAccessInstance()->getOrdersByAccountId(account.lock()->id());
		for (auto &ids : orderInfo)
		{
			shared_ptr<Order> newOrder = make_shared<Order>(ids[0], accountManager.getCustomer(ids[1]), accountManager.getMerchant(ids[2]));
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
	unsigned long id = DataSource::getDataAccessInstance()->createOrder(
			committer.lock()->id(), acceptor.lock()->id(), applianceType, detail
	);
	shared_ptr<Order> newOrder = make_shared<Order>(id, committer, acceptor);
	newOrder->initOrderState();
	return newOrder;
}*/

std::unique_ptr<OrderState> OrderFactory::loadOrderStates(shared_ptr<Order> &order)
{
	list<any> stateFactories;
	stateFactories = DataSource::getDataAccessInstance()->getOrderStateByOrderId(order->id());

	unique_ptr<OrderState> lastState;
	for(auto anyFactory : stateFactories)
	{
#define FACTORY_CAST(stateType)	\
	if(anyFactory.type() == typeid(shared_ptr<OrderStateFactory<stateType>>))	\
	{	\
		auto factory = std::any_cast<shared_ptr<OrderStateFactory<stateType>>>(anyFactory);\
		lastState = factory->makeStateForOrder(order, std::move(lastState));\
	}
		FACTORY_CAST(OrderUnreceivedState)
		else FACTORY_CAST(OrderStartRepairState)
		else FACTORY_CAST(OrderRejectedState)
		else FACTORY_CAST(OrderReceivedState)
		else FACTORY_CAST(OrderFinishedState)
		else FACTORY_CAST(OrderEndRepairState)
	}

	return lastState;
}

/*void OrderFactory::persistenceOrderState(std::weak_ptr<Order> &order)
{
	switch (order.lock()->currentState())
	{
		case OrderState::unreceivedState:
			break;
		case OrderState::receivedState:
			DATA_SOURCE_FROM::getInstance().orderReceived(order.lock()->id());
			break;
		case OrderState::startRepairState:
			DATA_SOURCE_FROM::getInstance().orderRepairing(order.lock()->id());
			break;
		case OrderState::endRepairState:
			DATA_SOURCE_FROM::getInstance().orderEndRepair(order.lock()->id(), order.lock()->transaction());
			break;
		case OrderState::finishedState:
			DATA_SOURCE_FROM::getInstance().orderFinished(order.lock()->id());
			break;
		case OrderState::rejectState:
			DATA_SOURCE_FROM::getInstance().orderRejected(order.lock()->id());
			break;
	}
}*/
std::shared_ptr<Order> OrderFactory::loadOrder(unsigned long id)
{
	AccountManagerProxy accountManagerProxy;
	unsigned long committerId = DataSource::getDataAccessInstance()->getOrderCommitter(id);
	unsigned long receiverId = DataSource::getDataAccessInstance()->getOrderReceiver(id);
	CustomerAccount *committer = accountManagerProxy.getCustomer(committerId);
	MerchantAccount *receiver = accountManagerProxy.getMerchant(receiverId);
	shared_ptr<Order> order = make_shared<Order>(id, committer, receiver);
	auto lastState = loadOrderStates(order);
	order->m_currentState = std::move(lastState);
	// TODO accounts?
	return order;
}
