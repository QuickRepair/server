#include "MerchantAccount.h"
#include "Order/Order.h"

using std::weak_ptr;					using std::list;
using std::shared_ptr;					using std::make_unique;

MerchantAccount::MerchantAccount(unsigned long id)
    :CustomerAccount{id}
{
	m_serviceType = make_unique<MerchantService>(this);
}

void MerchantAccount::orderWaitToBeAccept(std::weak_ptr<Order> order)
{
	m_orders[order.lock()->id()] = order.lock();
}

/*void MerchantAccount::acceptOrder(std::weak_ptr<Order> order)
{
	if(isMyUnreceivedOrder(order) && order.lock()->isNotReceived())
	{
		order.lock()->receivedBy(weak_from_this());
		m_processedOrders.push_back(order.lock());
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
	{
		order.lock()->reject();
		m_unreceivedOrders.remove(order.lock());
		m_processedOrders.push_back(order.lock());
	}
}

bool MerchantAccount::isMyUnreceivedOrder(std::weak_ptr<Order> order) const
{
	return m_unreceivedOrders.end() != std::find_if(m_unreceivedOrders.begin(), m_unreceivedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
}

bool MerchantAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return isMySubmittedOrder(order) || isMyUnreceivedOrder(order) || m_processedOrders.end() != std::find_if(m_processedOrders.begin(), m_processedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
}

bool MerchantAccount::isMySubmittedOrder(std::weak_ptr<Order> order) const
{
	return CustomerAccount::isMyOrder(order);
}

bool MerchantAccount::isMyProcessedOrder(std::weak_ptr<Order> order) const
{
	return m_processedOrders.end() != std::find_if(m_processedOrders.begin(), m_processedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
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
	for(auto &order : m_processedOrders)
		orderList.push_back(order);
	for(auto &order : m_unreceivedOrders)
		orderList.push_back(order);
	return orderList;
}*/

MerchantService *MerchantAccount::getService()
{
	return m_serviceType.get();
}

/*
void MerchantAccount::updateSupportedService(std::list<std::string> appliancType, double maxDistance)
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

void MerchantAccount::loadOrder(std::shared_ptr<Order> order)
{
	if(!isMyOrder(order))
	{
		if (order->currentState() == OrderState::States::unreceivedState)
			m_unreceivedOrders.push_back(order);
		else if(order->acceptorId() == id())
			m_processedOrders.push_back(order);
		else
			CustomerAccount::loadOrder(order);
	}
}

void MerchantAccount::loadServiceType(std::unique_ptr<MerchantService> &&service)
{
	m_serviceType = std::move(service);
}*/
