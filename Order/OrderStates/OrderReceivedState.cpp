#include "OrderReceivedState.h"
#include "OrderStartRepairState.h"
#include "OrderFinishedState.h"
#include "../Order.h"

using std::chrono::system_clock;		using std::runtime_error;
using std::make_shared;

OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver)
	: m_order{std::move(order)}, m_receiver{std::move(receiver)}, m_lastState{std::move(lastState)}
{
	m_stateChangeDate = system_clock::now();
}

OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver, std::chrono::system_clock::time_point date)
	: m_order{std::move(order)}, m_receiver{std::move(receiver)}, m_lastState{std::move(lastState)}, m_stateChangeDate{date}
{}

void OrderReceivedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw runtime_error("Order state not fit");
}

void OrderReceivedState::startRepair()
{
	m_order.lock()->m_receivedState = shared_from_this();
	m_order.lock()->setState(make_shared<OrderStartRepairState>(m_order, weak_from_this()));
}

void OrderReceivedState::endRepair(double transactionPrice)
{
	throw runtime_error("Order state not fit");
}

void OrderReceivedState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, weak_from_this()));
}

AcceptableOrderPriceRange OrderReceivedState::priceRange() const
{
	return m_lastState.lock()->priceRange();
}

double OrderReceivedState::transaction() const
{
	throw runtime_error("Order state not fit");
}

void OrderReceivedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw runtime_error("Order state not fit");
}

OrderEvaluate OrderReceivedState::evaluate() const
{
	throw runtime_error("Order state not fit");
}

std::chrono::system_clock::time_point OrderReceivedState::date() const
{
	return m_stateChangeDate;
}