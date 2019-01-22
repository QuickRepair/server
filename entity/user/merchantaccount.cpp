#include "merchantaccount.h"
#include "addressinformation.h"
#include "merchantservertype.h"
#include "../order/order.h"
#include <algorithm>

MerchantAccount::MerchantAccount(unsigned long id, std::string name, std::string password, std::string email)
    :User{id, std::move(name), std::move(password), std::move(email)}
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
	return m_orders.end() != std::find(m_orders.begin(), m_orders.end(), order.lock());
}

std::list<std::shared_ptr<Order>> MerchantAccount::myOrders() const
{
	return m_orders;
}

void MerchantAccount::loadAddress(std::list<std::shared_ptr<AddressInformation>> address)
{
	m_address = std::move(address.front());
}

void MerchantAccount::loadOrders(std::list<std::shared_ptr<Order>> orders)
{
	m_orders.clear();
	for(auto &element : orders)
		m_orders.push_back(element);
}