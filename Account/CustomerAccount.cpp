#include "CustomerAccount.h"
#include "ContactInformation.h"
#include "../Order/OrderStates/AcceptableOrderPriceRange.h"
#include "../Factories/OrderFactory.h"
#include "../Order/Order.h"
#include "../Order/OrderStates/OrderEvaluate.h"
#include <algorithm>

using std::weak_ptr;

CustomerAccount::CustomerAccount(unsigned long id, std::string userName, std::string password)
    :Account{id, std::move(password), std::move(userName)}
{}

void CustomerAccount::iAmPublishAnOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		m_orders.push_back(order.lock());
}

void CustomerAccount::cancelOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->orderFinished();
}

void CustomerAccount::evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate)
{
	if(isMyOrder(order))
		order.lock()->setEvaluate(evaluate);
}

void CustomerAccount::payTheOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->orderFinished();
}

bool CustomerAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return m_orders.end() != std::find_if(m_orders.begin(), m_orders.end(),
			[&order](weak_ptr<Order> a){ return a.lock() == order.lock(); });
}

std::list<std::weak_ptr<Order>> CustomerAccount::myOrders() const
{
	return m_orders;
}

void CustomerAccount::loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info)
{
	m_contactInfo.clear();
	for(auto &element : info)
		m_contactInfo.push_back(element);
}

void CustomerAccount::loadOrders(std::list<std::weak_ptr<Order>> orders)
{
	m_orders.clear();
	for(auto &element : orders)
		m_orders.push_back(element);
}