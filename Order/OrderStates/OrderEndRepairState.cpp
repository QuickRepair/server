#include "OrderEndRepairState.h"
#include "OrderFinishedState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.h"

using std::chrono::system_clock;					using std::make_shared;

OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, double transactionPrice)
		: OrderState(std::move(order), system_clock::now()), m_transaction{transactionPrice}, m_lastState{std::move(lastState)}
{}

OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, double transactionPrice, std::chrono::system_clock::time_point date)
	: OrderState(std::move(order), date), m_transaction{transactionPrice}, m_lastState{std::move(lastState)}
{}

void OrderEndRepairState::reject()
{
	throw OrderNotAtRightState("At end repair state, can not reject");
}

void OrderEndRepairState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

void OrderEndRepairState::startRepair()
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

void OrderEndRepairState::endRepair(double transactionPrice)
{
	throw OrderNotAtRightState("At end repair state, can not end repair");
}

void OrderEndRepairState::payTheOrder()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, shared_from_this()));
}

void OrderEndRepairState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, shared_from_this()));
}

AcceptableOrderPriceRange OrderEndRepairState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderEndRepairState::transaction() const
{
	return m_transaction;
}

void OrderEndRepairState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("At end repair state, can not set evaluate");
}

OrderEvaluate OrderEndRepairState::evaluate() const
{
	throw OrderNotAtRightState("At end repair state, no evaluate");
}

OrderState::States OrderEndRepairState::atState() const
{
	return OrderState::States::endRepairState;
}

std::chrono::system_clock::time_point OrderEndRepairState::createDate() const
{
	return m_lastState->createDate();
}

std::chrono::system_clock::time_point OrderEndRepairState::rejectDate() const
{
	throw OrderNotAtRightState("At end repair state, no reject date");
}

std::chrono::system_clock::time_point OrderEndRepairState::receiveDate() const
{
	return m_lastState->receiveDate();
}

std::chrono::system_clock::time_point OrderEndRepairState::startRepairDate() const
{
	return m_lastState->startRepairDate();
}

std::chrono::system_clock::time_point OrderEndRepairState::endRepairDate() const
{
	return m_stateChangeDate;
}

std::chrono::system_clock::time_point OrderEndRepairState::finishDate() const
{
	throw OrderNotAtRightState("At end repair state, no finish date");
}