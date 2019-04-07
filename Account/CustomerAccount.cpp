#include "CustomerAccount.h"
#include "ContactInformation.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Factories/OrderFactory.h"
#include "Order/Order.h"
#include "Order/OrderStates/OrderEvaluate.h"
#include <algorithm>

using std::weak_ptr;						using std::list;

CustomerAccount::CustomerAccount(unsigned long id, std::string account, std::string password)
    :Account{id, std::move(account), std::move(password)}
{}

void CustomerAccount::iAmPublishAnOrder(std::weak_ptr<Order> order)
{
	if(!isMyOrder(order))
		m_orders.push_back(order.lock());
}

void CustomerAccount::cancelOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->finished();
}

void CustomerAccount::evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate)
{
	if(isMyOrder(order))
		order.lock()->setEvaluate(evaluate);
}

void CustomerAccount::payTheOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->pay();
}

bool CustomerAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return m_orders.end() != std::find_if(m_orders.begin(), m_orders.end(),
			[&order](weak_ptr<Order> a){ return a.lock() == order.lock(); });
}

std::list<std::weak_ptr<Order>> CustomerAccount::myOrdersList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_orders)
		orderList.push_back(order);
	return orderList;
}

void CustomerAccount::loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info)
{
	m_contactInfo.clear();
	for(auto &element : info)
		m_contactInfo.push_back(element);
}

void CustomerAccount::loadOrder(std::shared_ptr<Order> order)
{
	m_orders.push_back(order);
}