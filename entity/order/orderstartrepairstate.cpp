#include "orderstartrepairstate.h"
#include "orderfinishedstate.h"
#include "orderendrepairstate.h"
#include "order.h"

using std::chrono::system_clock;		using std::runtime_error;
using std::make_shared;

OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState)
		: m_order{std::move(order)}, m_lastState{std::move(lastState)}
{
	m_stateChangeDate = system_clock::now();
}

OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, std::chrono::system_clock::time_point date)
	: m_order{std::move(order)}, m_lastState{std::move(lastState)}, m_stateChangeDate{date}
{}

void OrderStartRepairState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw runtime_error("Order state not fit");
}

void OrderStartRepairState::startRepair()
{
	throw runtime_error("Order state not fit");
}

void OrderStartRepairState::endRepair(double transactionPrice)
{
	m_order.lock()->m_startRepairState = shared_from_this();
	m_order.lock()->setState(make_shared<OrderEndRepairState>(m_order, weak_from_this(), transactionPrice));
}

void OrderStartRepairState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, weak_from_this()));
}

OrderPriceRange OrderStartRepairState::priceRange() const
{
	return m_lastState.lock()->priceRange();
}

double OrderStartRepairState::transaction() const
{
	throw runtime_error("Order state not fit");
}

void OrderStartRepairState::setEvaluate(Evaluate &evaluate)
{
	throw runtime_error("Order state not fit");
}

Evaluate OrderStartRepairState::evaluate() const
{
	throw runtime_error("Order state not fit");
}

std::chrono::system_clock::time_point OrderStartRepairState::date() const
{
	return m_stateChangeDate;
}