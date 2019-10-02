#include "OrderReceivedState.h"
#include "OrderStartRepairState.h"
#include "OrderFinishedState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "DataSource/DataSource.hpp"

using std::chrono::system_clock;					using std::make_unique;

OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
	: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderReceivedState::reject()
{
	throw OrderNotAtRightState("At received state, can not reject");
}

std::unique_ptr<OrderState> OrderReceivedState::receive()
{
	throw OrderNotAtRightState("At received state, can not receive");
}

std::unique_ptr<OrderState> OrderReceivedState::startRepair()
{
	DataSource::getDataAccessInstance()->orderRepairing(m_order.lock()->id());
	return make_unique<OrderStartRepairState>(m_order,
											  [this]{ std::unique_ptr<OrderReceivedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

std::unique_ptr<OrderState> OrderReceivedState::endRepair([[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At received state, can not end repair");
}

std::unique_ptr<OrderState> OrderReceivedState::payTheOrder()
{
	throw OrderNotAtRightState("At received state, can not pay");
}

std::unique_ptr<OrderState> OrderReceivedState::orderFinished()
{
	DataSource::getDataAccessInstance()->orderFinished(m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order,
										   [this]{ std::unique_ptr<OrderReceivedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

AcceptableOrderPriceRange OrderReceivedState::priceRange() const
{
	return m_lastState->priceRange();
}

double OrderReceivedState::transaction() const
{
	throw OrderNotAtRightState("At received state, no transaction");
}

void OrderReceivedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("At received state, can not set evaluate");
}

OrderEvaluate OrderReceivedState::evaluate() const
{
	throw OrderNotAtRightState("At received state, no evaluate");
}

OrderState::States OrderReceivedState::atState() const
{
	return OrderState::States::receivedState;
}

std::chrono::system_clock::time_point OrderReceivedState::createDate() const
{
	return m_lastState->createDate();
}

std::chrono::system_clock::time_point OrderReceivedState::rejectDate() const
{
	throw OrderNotAtRightState("At received state, no reject date");
}

std::chrono::system_clock::time_point OrderReceivedState::receiveDate() const
{
	return DataSource::getDataAccessInstance()->getOrderReceiveDate(m_order.lock()->id());
}

std::chrono::system_clock::time_point OrderReceivedState::startRepairDate() const
{
	throw OrderNotAtRightState("At received state, no start repair date");
}

std::chrono::system_clock::time_point OrderReceivedState::endRepairDate() const
{
	throw OrderNotAtRightState("At received state, no end repair date");
}

std::chrono::system_clock::time_point OrderReceivedState::finishDate() const
{
	throw OrderNotAtRightState("At received state, no finish date");
}