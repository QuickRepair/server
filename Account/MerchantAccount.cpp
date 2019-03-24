#include "MerchantAccount.h"
#include "ContactInformation.h"
#include "MerchantServiceType.h"
#include "../Order/Order.h"
#include <algorithm>

using std::weak_ptr;

MerchantAccount::MerchantAccount(unsigned long id, std::string account, std::string password)
    :Account{id, std::move(account), std::move(password)}
{}

void MerchantAccount::acceptOrder(std::weak_ptr<Order> order)
{
	if(order.lock()->isNotReceived())
	{
		order.lock()->receivedBy(weak_from_this());
		m_orders.push_back(order.lock());
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

bool MerchantAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return m_orders.end() != std::find_if(m_orders.begin(), m_orders.end(),
			[&order](weak_ptr<Order> a){ return a.lock() == order.lock(); });
}

std::list<std::weak_ptr<Order>> MerchantAccount::myOrders() const
{
	return m_orders;
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

void MerchantAccount::loadOrders(std::list<std::weak_ptr<Order>> orders)
{
	m_orders.clear();
	for(auto &element : orders)
		m_orders.push_back(element);
}

void MerchantAccount::loadServiceType(std::shared_ptr<MerchantServiceType> service)
{
	m_serviceType = std::move(service);
}