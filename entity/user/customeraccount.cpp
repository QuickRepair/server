#include "customeraccount.h"
#include "addressinformation.h"
#include "../order/orderpricerange.h"
#include "../../database/orderfactory.h"
#include "../order/order.h"
#include "../order/evaluate.h"
#include <algorithm>

using std::make_shared;

CustomerAccount::CustomerAccount(unsigned long id, std::string name, std::string password, std::string email)
    :User{id, std::move(name), std::move(password), std::move(email)}
{
	m_orderFactory = make_shared<OrderFactory>();
}

void CustomerAccount::submitOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		m_orders.push_back(order.lock());
}

void CustomerAccount::cancelOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->orderFinished();
}

void CustomerAccount::evaluateTheOrder(std::weak_ptr<Order> order, Evaluate evaluate)
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
	return m_orders.end() != std::find(m_orders.begin(), m_orders.end(), order.lock());
}

std::list<std::shared_ptr<Order>> CustomerAccount::myOrders() const
{
	return m_orders;
}

void CustomerAccount::loadAddress(std::list<std::shared_ptr<AddressInformation>> address)
{
	m_address.clear();
	for(auto &element : address)
		m_address.push_back(element);
}

void CustomerAccount::loadOrders(std::list<std::shared_ptr<Order>> orders)
{
	m_orders.clear();
	for(auto &element : orders)
		m_orders.push_back(element);
}