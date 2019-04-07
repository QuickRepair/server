#include "OrderFactory.h"
#include "OrderStateFactories/OrderStateAbstractFactory.h"
#include "Database/DatabaseConnection.h"
#include "Database/OrderStateParameters.h"
#include "Order/Order.h"
#include "Order/OrderStates/OrderState.h"
#include "Account/ContactInformation.h"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Managers/AccountManager.h"
#include "Managers/OrderManager.h"
#include "Errors/QueryResultEmptyError.h"
#include <tuple>

using std::make_shared;			using std::get;
using std::shared_ptr;

void OrderFactory::readOrdersForAccount(std::weak_ptr<Account> account)
{
	try
	{
		auto orderInfo = DatabaseConnection::getInstance().queryOrderByAccountId(account.lock()->id());
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
				newOrder->m_currentState = getStates(newOrder, get<5>(ret));
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
	unsigned long id = DatabaseConnection::getInstance().createOrder(committer.lock()->id(), acceptor.lock()->id(), applianceType, detail);
	shared_ptr<Order> newOrder = make_shared<Order>(id, committer, applianceType, contactWay, detail);
	newOrder->initOrderState(range);
	return newOrder;
}

std::shared_ptr<OrderState> OrderFactory::getStates(shared_ptr<Order> &order, unsigned long lastStateId, unsigned call)
{
	try
	{
		std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters> stateInfo;
		if (call == 0)
			stateInfo = DatabaseConnection::getInstance().queryOrderStateByOrderIdAndStateId(order->id(), lastStateId);
		else
			stateInfo = DatabaseConnection::getInstance().queryOrderStateByOrderIdAndLastStateId(order->id(),
																								 lastStateId);

		shared_ptr<OrderStateAbstractFactory> stateFactory = get<0>(stateInfo);
		OrderStateParameters parameters = get<1>(stateInfo);
		if (parameters.lastStateId != 0)
			parameters.lastState = getStates(order, parameters.lastStateId);

		return stateFactory->makeStateForOrder(order, parameters);
	}
	catch (QueryResultEmptyError &e)
	{
		return nullptr;
	}
}