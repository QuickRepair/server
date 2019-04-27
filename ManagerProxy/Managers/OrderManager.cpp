#include "OrderManager.h"
#include "Factories/OrderFactory.h"
#include "Account/CustomerAccount.h"
#include "Account/MerchantAccount.h"
#include "Account/ContactInformation.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Order/Order.h"
#include "AccountManager.h"
#include "ManagerProxy/AccountManagerProxy.h"
#include <algorithm>

using std::make_shared;			using std::shared_ptr;
using std::string;				using std::find_if;
using std::list;

OrderManager &OrderManager::getInstance()
{
	static OrderManager instance;
	return instance;
}

OrderManager::OrderManager()
{
	m_factory = make_shared<OrderFactory>();
	m_accountManagerProxy = make_shared<AccountManagerProxy>();
}

std::weak_ptr<Order> OrderManager::publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
								std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range)
{
	shared_ptr<Order> newOrder = m_factory->createOrder(committer, acceptor, applianceType, contactWay, detail, range);
	m_orderList.push_back(newOrder);
	committer.lock()->iAmPublishAnOrder(newOrder);
	acceptor.lock()->orderWaitToBeAccept(newOrder);
	return newOrder;
}

void OrderManager::orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->acceptOrder(order);
}

void OrderManager::orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order)
{
	account.lock()->rejectOrder(order);
}

void OrderManager::orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->startRepair(order);
}

void OrderManager::orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order,
								  double transaction)
{
	acceptor.lock()->endRepair(order, transaction);
}

void OrderManager::orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order)
{
	committer.lock()->payTheOrder(order);
}

void OrderManager::loadAllOrderForAccount(std::weak_ptr<Account> account)
{
	list<shared_ptr<Order>> orderList;
	orderList = m_factory->getOrdersListForAccount(account);
	for(auto &order : orderList)
	{
		auto customer = m_accountManagerProxy->getOrLoadCustomer(order->committerId());
		auto merchant = m_accountManagerProxy->getOrLoadMerchant(order->acceptorId());

		customer.lock()->loadOrder(order);
		merchant.lock()->loadOrder(order);
		m_orderList.push_back(order);
	}
}

std::weak_ptr<Order> OrderManager::getOrder(unsigned long id)
{
	auto it = find_if(m_orderList.begin(), m_orderList.end(),
			[&id](shared_ptr<Order> order){ return order->id() == id; });
	return it == m_orderList.end() ? nullptr : *it;
}