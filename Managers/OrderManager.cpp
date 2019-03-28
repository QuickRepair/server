#include "OrderManager.h"
#include "../Factories/OrderFactory.h"
#include "../Account/CustomerAccount.h"
#include "../Account/MerchantAccount.h"
#include "../Account/ContactInformation.h"
#include "../Order/OrderStates/AcceptableOrderPriceRange.h"

using std::make_shared;			using std::shared_ptr;
using std::string;

OrderManager &OrderManager::getInstance()
{
	static OrderManager instance;
	return instance;
}

OrderManager::OrderManager()
{
	m_factory = make_shared<OrderFactory>();
}

void OrderManager::publishOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
								std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range)
{
	shared_ptr<Order> newOrder = m_factory->createOrder(committer, acceptor, applianceType, contactWay, detail, range);
	m_orders.push_back(newOrder);
	committer.lock()->iAmPublishAnOrder(newOrder);
	acceptor.lock()->acceptOrder(newOrder);
}

void OrderManager::requestStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->startRepair(order);
}

void OrderManager::requestEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction)
{
	acceptor.lock()->endRepair(order, transaction);
}

void OrderManager::requestPayOrder(std::weak_ptr<CustomerAccount> &commiter, std::weak_ptr<Order> &order)
{
	commiter.lock()->payTheOrder(order);
}

void OrderManager::requestAcceptOrder(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->acceptOrder(order);
}

std::list<std::weak_ptr<Order>> OrderManager::requestQueryOrder(std::weak_ptr<Account> user)
{
	return user.lock()->myOrders();
}
