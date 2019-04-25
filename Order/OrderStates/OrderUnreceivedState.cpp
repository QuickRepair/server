#include "OrderUnreceivedState.h"
#include "OrderReceivedState.h"
#include "Order/Order.h"
#include "OrderFinishedState.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "OrderRejectedState.h"

using std::make_shared;						using std::chrono::system_clock;

OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range)
	: OrderState(std::move(order), system_clock::now()), m_range{range}
{}

OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range, std::chrono::system_clock::time_point date)
	: OrderState(std::move(order), date), m_range{range}
{}

void OrderUnreceivedState::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	m_order.lock()->m_acceptor = receiver;
	m_order.lock()->setState(make_shared<OrderReceivedState>(m_order, shared_from_this(), receiver));
}

void OrderUnreceivedState::reject()
{
	m_order.lock()->setState(make_shared<OrderRejectedState>(m_order, shared_from_this()));
}

void OrderUnreceivedState::startRepair()
{
	throw OrderNotAtRightState("At unreceived state, can not switch to start repair state");
}

void OrderUnreceivedState::endRepair(double transactionPrice)
{
	throw OrderNotAtRightState("At unreceived state, can not switch to end repair state");
}

void OrderUnreceivedState::payTheOrder()
{
	throw OrderNotAtRightState("At unreceived state, can not pay");
}

void OrderUnreceivedState::orderFinished()
{
	m_order.lock()->setState(make_shared<OrderFinishedState>(m_order, shared_from_this()));
}

AcceptableOrderPriceRange OrderUnreceivedState::priceRange() const
{
	return m_range;
}

double OrderUnreceivedState::transaction() const
{
	throw OrderNotAtRightState("At unreceived state, no transaction");
}

void OrderUnreceivedState::setEvaluate(OrderEvaluate &evaluate)
{
	throw OrderNotAtRightState("At unreceived state, can not set evaluate");
}

OrderEvaluate OrderUnreceivedState::evaluate() const
{
	throw OrderNotAtRightState("At unreceived state, no evaluate");
}

OrderState::States OrderUnreceivedState::atState() const
{
	return OrderState::States::unreceivedState;
}

std::chrono::system_clock::time_point OrderUnreceivedState::createDate() const
{
	return m_stateChangeDate;
}

std::chrono::system_clock::time_point OrderUnreceivedState::rejectDate() const
{
	throw OrderNotAtRightState("At unreceived state, no reject date");
}

std::chrono::system_clock::time_point OrderUnreceivedState::receiveDate() const
{
	throw OrderNotAtRightState("At unreceived state, no receive date");
}

std::chrono::system_clock::time_point OrderUnreceivedState::startRepairDate() const
{
	throw OrderNotAtRightState("At unreceived state, no start repair date");
}

std::chrono::system_clock::time_point OrderUnreceivedState::endRepairDate() const
{
	throw OrderNotAtRightState("At unreceived state, no end repair date");
}

std::chrono::system_clock::time_point OrderUnreceivedState::finishDate() const
{
	throw OrderNotAtRightState("At unreceived state, no finish date");
}