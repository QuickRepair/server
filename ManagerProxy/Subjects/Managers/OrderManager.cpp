#include "OrderManager.h"
#include "Factories/OrderFactory.hpp"
#include "DataSource/DataSource.hpp"

using std::make_unique;

OrderManager &OrderManager::getInstance()
{
	static OrderManager instance;
	return instance;
}

OrderManager::OrderManager()
{
	m_factory = make_unique<OrderFactory>();
//	m_accountManagerProxy = make_shared<AccountManagerProxy>();
}

std::shared_ptr<Order> OrderManager::createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail)
{
	unsigned long orderId = DataSource::getDataAccessInstance()->createOrder(committerId, acceptorId, applianceType, detail);
	return m_factory->loadOrder(orderId);
}

std::weak_ptr<Order> OrderManager::getOrder(unsigned long id)
{
	return m_orders[id];
}

/*std::weak_ptr<Order> OrderManager::publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
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

		if(!isTheOrderAlreadyLoaded(order, customer) && !isTheOrderAlreadyLoaded(order, merchant))
			m_orderList.push_back(order);
		if(!isTheOrderAlreadyLoaded(order, customer))
			customer.lock()->loadOrder(order);
		if(!isTheOrderAlreadyLoaded(order, merchant))
			merchant.lock()->loadOrder(order);
	}
}

std::weak_ptr<Order> OrderManager::getOrder(unsigned long id)
{
	auto it = find_if(m_orderList.begin(), m_orderList.end(),
			[&id](shared_ptr<Order> order){ return order->id() == id; });
	return it == m_orderList.end() ? nullptr : *it;
}

bool OrderManager::isTheOrderAlreadyLoaded(std::weak_ptr<Order> order, std::weak_ptr<Account> account)
{
	return account.lock()->isMyOrder(order);
}*/
