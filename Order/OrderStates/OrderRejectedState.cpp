//
// Created by gaojian on 19-3-30.
//

#include "OrderRejectedState.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "OrderFinishedState.h"
#include "Order/Order.h"
#include "DataSource/DataSource.hpp"

using std::make_unique;						using std::chrono::system_clock;

OrderRejectedState::OrderRejectedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderRejectedState::receive()
{
	throw OrderNotAtRightState("At reject state, can not switch to received state");
}

std::unique_ptr<OrderState> OrderRejectedState::reject()
{
	throw OrderNotAtRightState("At reject state, can not switch to reject state");
}

std::unique_ptr<OrderState> OrderRejectedState::startRepair()
{
	throw OrderNotAtRightState("At reject state, can not switch to start repair state");
}

std::unique_ptr<OrderState> OrderRejectedState::endRepair([[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At reject state, can not switch to end repair state");
}

std::unique_ptr<OrderState> OrderRejectedState::payTheOrder()
{
	throw OrderNotAtRightState("At reject state, can not pay");
}

std::unique_ptr<OrderState> OrderRejectedState::orderFinished()
{
	DataSource::getDataAccessInstance()->orderFinished(m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order,
										   [this]{ std::unique_ptr<OrderRejectedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
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
	return DataSource::getDataAccessInstance()->getOrderRejectDate(m_order.lock()->id());
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