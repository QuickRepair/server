#include "OrderFinishedState.h"

using std::chrono::system_clock;		using std::runtime_error;
using std::make_shared;

OrderFinishedState::OrderFinishedState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState)
		: m_order{order}, m_lastState{lastState}
{}

void OrderFinishedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw runtime_error("Order state not fit");
}

void OrderFinishedState::startRepair()
{
	throw runtime_error("Order state not fit");
}

void OrderFinishedState::endRepair(double transactionPrice)
{
	throw runtime_error("Order state not fit");
}

void OrderFinishedState::orderFinished()
{
	throw runtime_error("Order state not fit");
}

AcceptableOrderPriceRange OrderFinishedState::priceRange() const
{
	return m_lastState.lock()->priceRange();
}

double OrderFinishedState::transaction() const
{
	return m_lastState.lock()->transaction();
}

void OrderFinishedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw runtime_error("Order state not fit");
}

OrderEvaluate OrderFinishedState::evaluate() const
{
	return m_lastState.lock()->evaluate();
}

std::chrono::system_clock::time_point OrderFinishedState::date() const
{
	throw runtime_error("Order state not fit");
}