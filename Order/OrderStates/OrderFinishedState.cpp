#include "OrderFinishedState.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "DataSource/DataSource.hpp"
#include "Order/Order.h"

using std::chrono::system_clock;				using std::make_shared;

OrderFinishedState::OrderFinishedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderFinishedState::reject()
{
	throw OrderNotAtRightState("At finish state, can not reject");
}

std::unique_ptr<OrderState> OrderFinishedState::receive()
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

std::unique_ptr<OrderState> OrderFinishedState::startRepair()
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

std::unique_ptr<OrderState> OrderFinishedState::endRepair([[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At finish state, can not end repair");
}

std::unique_ptr<OrderState> OrderFinishedState::payTheOrder()
{
	throw OrderNotAtRightState("At finish state, can not pay");
}

std::unique_ptr<OrderState> OrderFinishedState::orderFinished()
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
	DataSource::getDataAccessInstance()->setOrderEvaluate(evaluate);
}

OrderEvaluate OrderFinishedState::evaluate() const
{
	return DataSource::getDataAccessInstance()->getOrderEvaluate(m_order.lock()->id());
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
	return DataSource::getDataAccessInstance()->getOrderFinishDate(m_order.lock()->id());
}