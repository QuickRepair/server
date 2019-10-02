#include "OrderUnreceivedState.h"
#include "OrderReceivedState.h"
#include "Order/Order.h"
#include "OrderFinishedState.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "OrderRejectedState.h"
#include "DataSource/DataSource.hpp"
#include "Account/MerchantAccount.h"

using std::make_unique;						using std::chrono::system_clock;

OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
	: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderUnreceivedState::receive()
{
	DataSource::getDataAccessInstance()->orderReceived(m_order.lock()->id());
	return make_unique<OrderReceivedState>(m_order,
										   [this]{ std::unique_ptr<OrderUnreceivedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

std::unique_ptr<OrderState> OrderUnreceivedState::reject()
{
	DataSource::getDataAccessInstance()->orderRejected(m_order.lock()->id());
	return make_unique<OrderRejectedState>(m_order,
										   [this]{ std::unique_ptr<OrderUnreceivedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

std::unique_ptr<OrderState> OrderUnreceivedState::startRepair()
{
	throw OrderNotAtRightState("At unreceived state, can not switch to start repair state");
}

std::unique_ptr<OrderState> OrderUnreceivedState::endRepair([[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At unreceived state, can not switch to end repair state");
}

std::unique_ptr<OrderState> OrderUnreceivedState::payTheOrder()
{
	throw OrderNotAtRightState("At unreceived state, can not pay");
}

std::unique_ptr<OrderState> OrderUnreceivedState::orderFinished()
{
	DataSource::getDataAccessInstance()->orderFinished(m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order,
										   [this]{ std::unique_ptr<OrderUnreceivedState> ownershipTransfer; ownershipTransfer.reset(this); return ownershipTransfer; }()
	);
}

AcceptableOrderPriceRange OrderUnreceivedState::priceRange() const
{
	return DataSource::getDataAccessInstance()->getAcceptablePriceRange(m_order.lock()->id());
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
	return DataSource::getDataAccessInstance()->getOrderCreateDate(m_order.lock()->id());
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