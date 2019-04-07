#include "OrderReceivedState.h"
#include "OrderStartRepairState.h"
#include "OrderFinishedState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.h"

using std::chrono::system_clock;					using std::make_shared;

OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver)
	: OrderState(std::move(order), system_clock::now()),
	m_receiver{std::move(receiver)}, m_lastState{std::move(lastState)}
{}

OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver, std::chrono::system_clock::time_point date)
	: OrderState(std::move(order), date),
	m_receiver{std::move(receiver)}, m_lastState{std::move(lastState)}
{}

void OrderReceivedState::reject()
{
	throw OrderNotAtRightState("At received state, can not reject");
}

void OrderReceivedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	throw OrderNotAtRightState("At received state, can not receive");
}

void OrderReceivedState::startRepair()
{
	m_order.lock()->setState(make_shared<OrderStartRepairState>(m_order, shared_from_this()));
}

void OrderReceivedState::endRepair(double transactionPrice)
{
	throw OrderNotAtRightState("At received state, can not end repair");
}

void OrderReceivedState::payTheOrder()
{
	throw OrderNotAtRightState("At received state, can not pay");
}

void OrderReceivedState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, shared_from_this()));
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
	return m_stateChangeDate;
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