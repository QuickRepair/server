#include "MerchantAccount.h"
#include "ContactInformation.h"
#include "MerchantServiceType.h"
#include "../Order/Order.h"
#include <algorithm>

using std::weak_ptr;					using std::list;
using std::shared_ptr;

MerchantAccount::MerchantAccount(unsigned long id, std::string account, std::string password)
    :Account{id, std::move(account), std::move(password)}
{}

void MerchantAccount::orderWaitToBeAccept(std::shared_ptr<Order> order)
{
	m_unreceivedOrders.push_back(order);
}

void MerchantAccount::acceptOrder(std::weak_ptr<Order> order)
{
	if(isMyUnreceivedOrder(order) && order.lock()->isNotReceived())
	{
		order.lock()->receivedBy(weak_from_this());
		m_receivedOrders.push_back(order.lock());
		m_unreceivedOrders.remove(order.lock());
	}
}

void MerchantAccount::startRepair(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->startRepair();
}

void MerchantAccount::endRepair(std::weak_ptr<Order> order, double transaction)
{
	if(isMyOrder(order))
		order.lock()->endRepair(transaction);
}

void MerchantAccount::rejectOrder(std::weak_ptr<Order> order)
{
	if(isMyUnreceivedOrder(order))
		order.lock()->reject();
}

bool MerchantAccount::isMyUnreceivedOrder(std::weak_ptr<Order> order)
{
	return m_unreceivedOrders.end() != std::find_if(m_unreceivedOrders.begin(), m_unreceivedOrders.end(),
			[&order](shared_ptr<Order> a){ return a == order.lock(); });
}

bool MerchantAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return m_receivedOrders.end() != std::find_if(m_receivedOrders.begin(), m_receivedOrders.end(),
			[&order](shared_ptr<Order> a){ return a == order.lock(); });
}

std::list<std::weak_ptr<Order>> MerchantAccount::myUnreceivedOrderList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_unreceivedOrders)
		orderList.push_back(order);
	return orderList;
}

std::list<std::weak_ptr<Order>> MerchantAccount::myOrdersList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_receivedOrders)
		orderList.push_back(order);
	for(auto &order : m_unreceivedOrders)
		orderList.push_back(order);
	return orderList;
}

std::weak_ptr<MerchantServiceType> MerchantAccount::supportedServiceType()
{
	return m_serviceType;
}

void MerchantAccount::updateSupportedService(std::list<std::string> appliancType, int maxDistance)
{
	m_serviceType->setMaxRepairDistance(maxDistance);
	m_serviceType->setSupportApplianceType(appliancType);
}

void MerchantAccount::loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info)
{
	m_contactInfo.clear();
	for(auto &element : info)
		m_contactInfo.push_back(element);
}

void MerchantAccount::loadOrders(std::list<std::shared_ptr<Order>> orders)
{
	m_receivedOrders.clear();
	m_receivedOrders = std::move(orders);
}

void MerchantAccount::loadServiceType(std::shared_ptr<MerchantServiceType> service)
{
	m_serviceType = std::move(service);
}