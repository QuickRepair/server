//
// Created by gaojian on 19-3-30.
//

#include "OrderRejectedState.h"
#include "../../Errors/OrderNotAtRightState.h"

using std::make_shared;						using std::chrono::system_clock;

OrderRejectedState::OrderRejectedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState)
		: OrderState(std::move(order), system_clock::now()), m_lastState{std::move(lastState)}
{}

OrderRejectedState::OrderRejectedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::chrono::system_clock::time_point date)
		: OrderState(std::move(order), date), m_lastState{std::move(lastState)}
{}

void OrderRejectedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw OrderNotAtRightState("At reject state, can not switch to received state");
}

void OrderRejectedState::reject()
{
	throw OrderNotAtRightState("At reject state, can not switch to reject state");
}

void OrderRejectedState::startRepair()
{
	throw OrderNotAtRightState("At reject state, can not switch to start repair state");
}

void OrderRejectedState::endRepair(double transactionPrice)
{
	throw OrderNotAtRightState("At reject state, can not switch to end repair state");
}

void OrderRejectedState::orderFinished()
{
	throw OrderNotAtRightState("At reject state, can not switch to finish state");
}

AcceptableOrderPriceRange OrderRejectedState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderRejectedState::transaction() const
{
	throw OrderNotAtRightState("At reject state, no transaction");
}

void OrderRejectedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("At reject state, can not set evaluate");
}

OrderEvaluate OrderRejectedState::evaluate() const
{
	throw OrderNotAtRightState("At reject state, no evaluate");
}

OrderState::States OrderRejectedState::atState() const
{
	return OrderState::States::rejectState;
}

std::chrono::system_clock::time_point OrderRejectedState::createDate() const
{
	return m_lastState->createDate();
}

std::chrono::system_clock::time_point OrderRejectedState::rejectDate() const
{
	return m_stateChangeDate;
}

std::chrono::system_clock::time_point OrderRejectedState::receiveDate() const
{
	throw OrderNotAtRightState("At reject state, no receive date");
}

std::chrono::system_clock::time_point OrderRejectedState::startRepairDate() const
{
	throw OrderNotAtRightState("At reject state, no start repair date");
}

std::chrono::system_clock::time_point OrderRejectedState::endRepairDate() const
{
	throw OrderNotAtRightState("At reject state, no end repair date");
}

std::chrono::system_clock::time_point OrderRejectedState::finishDate() const
{
	throw OrderNotAtRightState("At reject state, no finish date");
}