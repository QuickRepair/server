#include "CustomerAccount.h"
#include "ContactInformation.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Factories/OrderFactory.hpp"
#include "Order/Order.h"
#include "Order/OrderStates/OrderEvaluate.h"
#include <algorithm>

using std::weak_ptr;						using std::list;

CustomerAccount::CustomerAccount(unsigned long id)
    :Account{id}
{}

void CustomerAccount::iAmPublishAnOrder(std::weak_ptr<Order> order)
{
	m_orders[order.lock()->id()] = order.lock();
}

std::list<std::weak_ptr<Order>> CustomerAccount::getOrderList() const
{
	std::list<std::weak_ptr<Order>> orderList;
	for (auto &o : m_orders)
		orderList.emplace_back(o.second);
	return orderList;
}

/*
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
			[&order](weak_ptr<Order> a){ return a.lock()->id() == order.lock()->id(); });
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
	if(!isMyOrder(order))
		m_orders.push_back(order);
}*/
