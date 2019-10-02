#include "OrderEndRepairState.h"
#include "OrderFinishedState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "DataSource/DataSource.hpp"

using std::chrono::system_clock;					using std::make_unique;

OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderEndRepairState::reject()
{
	throw OrderNotAtRightState("At end repair state, can not reject");
}

std::unique_ptr<OrderState> OrderEndRepairState::receive()
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

std::unique_ptr<OrderState> OrderEndRepairState::startRepair()
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

std::unique_ptr<OrderState> OrderEndRepairState::endRepair([[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At end repair state, can not end repair");
}

std::unique_ptr<OrderState> OrderEndRepairState::payTheOrder()
{
	DataSource::getDataAccessInstance()->orderPayed(m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order,
										   [this]{ std::unique_ptr<OrderEndRepairState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

std::unique_ptr<OrderState> OrderEndRepairState::orderFinished()
{
	return make_unique<OrderFinishedState>(m_order,
			[this]{ std::unique_ptr<OrderEndRepairState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

AcceptableOrderPriceRange OrderEndRepairState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderEndRepairState::transaction() const
{
	return DataSource::getDataAccessInstance()->getOrderTransaction(m_order.lock()->id());
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
	return DataSource::getDataAccessInstance()->getOrderEndRepairDate(m_order.lock()->id());
}

std::chrono::system_clock::time_point OrderEndRepairState::finishDate() const
{
	throw OrderNotAtRightState("At end repair state, no finish date");
}