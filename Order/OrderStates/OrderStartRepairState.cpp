#include "OrderStartRepairState.h"
#include "OrderFinishedState.h"
#include "OrderEndRepairState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "DataSource/DataSource.hpp"

using std::chrono::system_clock;				using std::make_unique;

OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderStartRepairState::reject()
{
	throw OrderNotAtRightState("At start repair state, can not reject");
}

std::unique_ptr<OrderState> OrderStartRepairState::receive()
{
	throw OrderNotAtRightState("At start repair state, can not receive");
}

std::unique_ptr<OrderState> OrderStartRepairState::startRepair()
{
	throw OrderNotAtRightState("At start repair state, can not start");
}

std::unique_ptr<OrderState> OrderStartRepairState::endRepair(double transactionPrice)
{
	DataSource::getDataAccessInstance()->orderEndRepair(m_order.lock()->id(), transactionPrice);
	return make_unique<OrderEndRepairState>(m_order,
			[this]{ std::unique_ptr<OrderStartRepairState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

std::unique_ptr<OrderState> OrderStartRepairState::payTheOrder()
{
	throw OrderNotAtRightState("At start repair state, can not pay");
}

std::unique_ptr<OrderState> OrderStartRepairState::orderFinished()
{
	DataSource::getDataAccessInstance()->orderFinished(m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order,
										   [this]{ std::unique_ptr<OrderStartRepairState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
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
	return DataSource::getDataAccessInstance()->getOrderStartRepairDate(m_order.lock()->id());
}

std::chrono::system_clock::time_point OrderStartRepairState::endRepairDate() const
{
	throw OrderNotAtRightState("At start repair state, no end repair date");
}

std::chrono::system_clock::time_point OrderStartRepairState::finishDate() const
{
	throw OrderNotAtRightState("At start repair state, no finish date");
}