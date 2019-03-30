#include "OrderFinishedState.h"
#include "../../Errors/OrderNotAtRightState.h"

using std::chrono::system_clock;				using std::make_shared;

OrderFinishedState::OrderFinishedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState)
		: OrderState(order, system_clock::now()), m_lastState{lastState}
{}

OrderFinishedState::OrderFinishedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::chrono::system_clock::time_point date)
		: OrderState(order, date), m_lastState{lastState}
{}

void OrderFinishedState::reject()
{
	throw OrderNotAtRightState("At finish state, can not reject");
}

void OrderFinishedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

void OrderFinishedState::startRepair()
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

void OrderFinishedState::endRepair(double transactionPrice)
{
	throw OrderNotAtRightState("At finish state, can not end repair");
}

void OrderFinishedState::orderFinished()
{
	throw OrderNotAtRightState("At finish state, can not finish");
}

AcceptableOrderPriceRange OrderFinishedState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderFinishedState::transaction() const
{
	return m_lastState->transaction();
}

void OrderFinishedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("Order state not fit");
}

OrderEvaluate OrderFinishedState::evaluate() const
{
	return m_lastState->evaluate();
}

OrderState::States OrderFinishedState::atState() const
{
	return OrderState::States::finishedState;
}

std::chrono::system_clock::time_point OrderFinishedState::createDate() const
{
	return m_lastState->createDate();
}

std::chrono::system_clock::time_point OrderFinishedState::rejectDate() const
{
	throw OrderNotAtRightState("At end repair state, no reject date");
}

std::chrono::system_clock::time_point OrderFinishedState::receiveDate() const
{
	return m_lastState->receiveDate();
}

std::chrono::system_clock::time_point OrderFinishedState::startRepairDate() const
{
	return m_lastState->startRepairDate();
}

std::chrono::system_clock::time_point OrderFinishedState::endRepairDate() const
{
	return m_lastState->endRepairDate();
}

std::chrono::system_clock::time_point OrderFinishedState::finishDate() const
{
	return m_stateChangeDate;
}