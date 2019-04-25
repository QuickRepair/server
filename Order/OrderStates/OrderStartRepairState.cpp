#include "OrderStartRepairState.h"
#include "OrderFinishedState.h"
#include "OrderEndRepairState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"

using std::chrono::system_clock;				using std::make_shared;

OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState)
		: OrderState(std::move(order), system_clock::now()), m_lastState{std::move(lastState)}
{}

OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::chrono::system_clock::time_point date)
	: OrderState(std::move(order), date), m_lastState{std::move(lastState)}
{}

void OrderStartRepairState::reject()
{
	throw OrderNotAtRightState("At start repair state, can not reject");
}

void OrderStartRepairState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw OrderNotAtRightState("At start repair state, can not receive");
}

void OrderStartRepairState::startRepair()
{
	throw OrderNotAtRightState("At start repair state, can not start");
}

void OrderStartRepairState::endRepair(double transactionPrice)
{
	m_order.lock()->setState(make_shared<OrderEndRepairState>(m_order, shared_from_this(), transactionPrice));
}

void OrderStartRepairState::payTheOrder()
{
	throw OrderNotAtRightState("At start repair state, can not pay");
}

void OrderStartRepairState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, shared_from_this()));
}

AcceptableOrderPriceRange OrderStartRepairState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderStartRepairState::transaction() const
{
	throw OrderNotAtRightState("At start repair state, no transaction");
}

void OrderStartRepairState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("At start repair state, can not set evaluate");
}

OrderEvaluate OrderStartRepairState::evaluate() const
{
	throw OrderNotAtRightState("At start repair state, no evaluate");
}

OrderState::States OrderStartRepairState::atState() const
{
	return OrderState::States::startRepairState;
}

std::chrono::system_clock::time_point OrderStartRepairState::createDate() const
{
	return m_lastState->createDate();
}

std::chrono::system_clock::time_point OrderStartRepairState::rejectDate() const
{
	throw OrderNotAtRightState("At start repair state, no reject date");
}

std::chrono::system_clock::time_point OrderStartRepairState::receiveDate() const
{
	return m_lastState->receiveDate();
}

std::chrono::system_clock::time_point OrderStartRepairState::startRepairDate() const
{
	return m_stateChangeDate;
}

std::chrono::system_clock::time_point OrderStartRepairState::endRepairDate() const
{
	throw OrderNotAtRightState("At start repair state, no end repair date");
}

std::chrono::system_clock::time_point OrderStartRepairState::finishDate() const
{
	throw OrderNotAtRightState("At start repair state, no finish date");
}