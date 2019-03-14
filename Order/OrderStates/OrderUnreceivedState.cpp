#include "OrderUnreceivedState.h"
#include "OrderReceivedState.h"
#include "../Order.h"
#include "OrderFinishedState.h"
#include <stdexcept>

using std::runtime_error;			using std::make_shared;
using std::chrono::system_clock;

OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range)
	: m_order{std::move(order)}, m_range{range}
{
	m_stateChangeDate = system_clock::now();
}

OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range, std::chrono::system_clock::time_point date)
	: m_order{std::move(order)}, m_range{range}, m_stateChangeDate{date}
{}

void OrderUnreceivedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	m_order.lock()->m_unreceivedState = shared_from_this();
	m_order.lock()->m_acceptor = receiver;
	m_order.lock()->setState(make_shared<OrderReceivedState>(m_order, weak_from_this(), receiver));
}

void OrderUnreceivedState::startRepair()
{
	throw runtime_error("Order state not fit");
}

void OrderUnreceivedState::endRepair(double transactionPrice)
{
	throw runtime_error("Order state not fit");
}

void OrderUnreceivedState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, weak_from_this()));
}

AcceptableOrderPriceRange OrderUnreceivedState::priceRange() const
{
	return m_range;
}

double OrderUnreceivedState::transaction() const
{
	throw runtime_error("Order state not fit");
}

void OrderUnreceivedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw runtime_error("Order state not fit");
}

OrderEvaluate OrderUnreceivedState::evaluate() const
{
	throw runtime_error("Order state not fit");
}

std::chrono::system_clock::time_point OrderUnreceivedState::date() const
{
	return m_stateChangeDate;
}
