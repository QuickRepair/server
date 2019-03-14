#include "OrderEndRepairState.h"
#include "OrderFinishedState.h"
#include "../Order.h"

using std::chrono::system_clock;		using std::runtime_error;
using std::make_shared;

OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, double transactionPrice)
		: m_order{std::move(order)}, m_transaction{transactionPrice}, m_lastState{std::move(lastState)}
{
	m_stateChangeDate = system_clock::now();
}

OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, double transactionPrice, OrderEvaluate evaluate, std::chrono::system_clock::time_point date)
	: m_order{std::move(order)}, m_transaction{transactionPrice}, m_lastState{std::move(lastState)}, m_evaluate{evaluate}, m_stateChangeDate{date}
{}

void OrderEndRepairState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw runtime_error("Order state not fit");
}

void OrderEndRepairState::startRepair()
{
	throw runtime_error("Order state not fit");
}

void OrderEndRepairState::endRepair(double transactionPrice)
{
	throw runtime_error("Order state not fit");
}

void OrderEndRepairState::orderFinished()
{
	m_order.lock()->m_endRepairState = shared_from_this();
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, weak_from_this()));
}

AcceptableOrderPriceRange OrderEndRepairState::priceRange() const
{
	return m_lastState.lock()->priceRange();
}

double OrderEndRepairState::transaction() const
{
	return m_transaction;
}

void OrderEndRepairState::setEvaluate(OrderEvaluate &evaluate)
{
	m_evaluate = evaluate;
}

OrderEvaluate OrderEndRepairState::evaluate() const
{
	return m_evaluate;
}

std::chrono::system_clock::time_point OrderEndRepairState::date() const
{
	return m_stateChangeDate;
}