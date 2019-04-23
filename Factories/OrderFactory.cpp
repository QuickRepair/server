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
#include "Managers/AccountManager.h"
#include "Errors/QueryResultEmptyError.h"
#include <tuple>

using std::make_shared;			using std::get;
using std::shared_ptr;			using std::list;
using std::tuple;

void OrderFactory::readOrdersForAccount(std::weak_ptr<Account> account)
{
	try
	{
		auto orderInfo = DataSourceFrom::getInstance().queryOrderByAccountId(account.lock()->id());
		for (auto &ret : orderInfo)
		{
			auto committer = AccountManager::getInstance().getCustomer(get<1>(ret));
			auto acceptor = AccountManager::getInstance().getMerchant(get<2>(ret));

			if (committer.lock() != nullptr || acceptor.lock() != nullptr)
			{
				if (committer.lock() == nullptr)
					committer = AccountManager::getInstance().loadCustomer(get<1>(ret));
				if (acceptor.lock() == nullptr)
					acceptor = AccountManager::getInstance().loadMerchant(get<2>(ret));
				ContactInformation tmp;
				shared_ptr<Order> newOrder = make_shared<Order>(get<0>(ret), committer, get<3>(ret), tmp, get<4>(ret));
				newOrder->m_currentState = getStates(newOrder);
				committer.lock()->loadOrder(newOrder);
				acceptor.lock()->loadOrder(newOrder);
			}
		}
	}
	catch (QueryResultEmptyError &e)
	{
	}
}

std::shared_ptr<Order> OrderFactory::createOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
												 std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range)
{
	unsigned long id = DataSourceFrom::getInstance().createOrder(committer.lock()->id(), acceptor.lock()->id(), applianceType, detail);
	shared_ptr<Order> newOrder = make_shared<Order>(id, committer, applianceType, contactWay, detail);
	newOrder->initOrderState(range);
	return newOrder;
}

std::shared_ptr<OrderState> OrderFactory::getStates(shared_ptr<Order> &order)
{
	/* TODO
	try
	{
		list<tuple<shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>> stateInfo;
		stateInfo = DataSourceFrom::getInstance().queryOrderStateByOrderId(order->id());

		shared_ptr<OrderStateAbstractFactory> stateFactory = get<0>(stateInfo);
		OrderStateParameters parameters = get<1>(stateInfo);
		if (parameters.lastStateId != 0)
			parameters.lastState = getStates(order, parameters.lastStateId);

		return stateFactory->makeStateForOrder(order, parameters);
	}
	catch (QueryResultEmptyError &e)
	{
		return nullptr;
	}*/
	return nullptr;
}
