//
// Created by gaojian on 19-4-25.
//

#include "ManagerProxy/Subjects/Managers/OrderManager.h"
#include "OrderManagerProxy.h"

std::shared_ptr<Order>
OrderManagerProxy::createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType,
							   std::string detail)
{
	return OrderManager::getInstance().createOrder(committerId, acceptorId, applianceType, detail);
}

std::weak_ptr<Order> OrderManagerProxy::getOrder(OrderId id)
{
	return OrderManager::getInstance().getOrder(id);
}

std::unordered_map<OrderId, std::shared_ptr<Order>> OrderManagerProxy::loadAccountOrders(AccountId id)
{
	return OrderManager::getInstance().loadAccountOrders(id);
}

/*std::weak_ptr<Order> OrderManagerProxy::publishOrder(std::weak_ptr<CustomerAccount> &committer,
									 std::weak_ptr<MerchantAccount> &acceptor, std::string &applianceType,
									 ContactInformation &contactWay, std::string &detail,
									 AcceptableOrderPriceRange &range)
{
	return OrderManager::getInstance().publishOrder(committer, acceptor, applianceType, contactWay, detail, range);
}

void OrderManagerProxy::orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	OrderManager::getInstance().orderAccepted(acceptor, order);
}

void OrderManagerProxy::orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order)
{
	OrderManager::getInstance().orderRejected(account, order);
}

void OrderManagerProxy::orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	OrderManager::getInstance().orderStartRepair(acceptor, order);
}

void OrderManagerProxy::orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order,
									   double transaction)
{
	OrderManager::getInstance().orderEndRepair(acceptor, order, transaction);
}

void OrderManagerProxy::orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order)
{
	OrderManager::getInstance().orderPayed(committer, order);
}

void OrderManagerProxy::loadAllOrderForAccount(std::weak_ptr<Account> account)
{
	OrderManager::getInstance().loadAllOrderForAccount(account);
}*/
