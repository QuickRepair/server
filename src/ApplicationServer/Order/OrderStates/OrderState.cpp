//
// Created by 17271 on 2019/9/29.
//

#include "Thread/ThreadPool.hpp"
#include "OrderState.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "DataSource/DataSource.hpp"

using std::make_unique;

OrderState::~OrderState()
{}

/// OrderUnreceivedState
OrderUnreceivedState::OrderUnreceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderUnreceivedState::receive(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderReceived, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderReceivedState>(m_order, std::move(current));
}

std::unique_ptr<OrderState> OrderUnreceivedState::reject(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderRejected, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderRejectedState>(m_order, std::move(current));
}

std::unique_ptr<OrderState> OrderUnreceivedState::startRepair(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At unreceived state, can not switch to start repair state");
}

std::unique_ptr<OrderState> OrderUnreceivedState::endRepair(std::unique_ptr<OrderState> &&current, [[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At unreceived state, can not switch to end repair state");
}

std::unique_ptr<OrderState> OrderUnreceivedState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At unreceived state, can not pay");
}

std::unique_ptr<OrderState> OrderUnreceivedState::orderFinished(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderFinished, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order, std::move(current));
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

/// OrderReceivedState
OrderReceivedState::OrderReceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderReceivedState::reject(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At received state, can not reject");
}

std::unique_ptr<OrderState> OrderReceivedState::receive(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At received state, can not receive");
}

std::unique_ptr<OrderState> OrderReceivedState::startRepair(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderRepairing, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderStartRepairState>(m_order, std::move(current));
}

std::unique_ptr<OrderState> OrderReceivedState::endRepair(std::unique_ptr<OrderState> &&current, [[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At received state, can not end repair");
}

std::unique_ptr<OrderState> OrderReceivedState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At received state, can not pay");
}

std::unique_ptr<OrderState> OrderReceivedState::orderFinished(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderFinished, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order, std::move(current));
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

/// OrderStartRepairState
OrderStartRepairState::OrderStartRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderStartRepairState::reject(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At start repair state, can not reject");
}

std::unique_ptr<OrderState> OrderStartRepairState::receive(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At start repair state, can not receive");
}

std::unique_ptr<OrderState> OrderStartRepairState::startRepair(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At start repair state, can not start");
}

std::unique_ptr<OrderState> OrderStartRepairState::endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice)
{
	ThreadPool::getInstance().submit(&DataAccess::orderEndRepair, DataSource::getDataAccessInstance(), m_order.lock()->id(), transactionPrice);
	return make_unique<OrderEndRepairState>(m_order, std::move(current));
}

std::unique_ptr<OrderState> OrderStartRepairState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At start repair state, can not pay");
}

std::unique_ptr<OrderState> OrderStartRepairState::orderFinished(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderFinished, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order, std::move(current));
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

/// OrderEndRepairState
OrderEndRepairState::OrderEndRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderEndRepairState::reject(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At end repair state, can not reject");
}

std::unique_ptr<OrderState> OrderEndRepairState::receive(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

std::unique_ptr<OrderState> OrderEndRepairState::startRepair(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At end repair state, can not receive");
}

std::unique_ptr<OrderState> OrderEndRepairState::endRepair(std::unique_ptr<OrderState> &&current, [[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At end repair state, can not end repair");
}

std::unique_ptr<OrderState> OrderEndRepairState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderPayed, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order, std::move(current));
}

std::unique_ptr<OrderState> OrderEndRepairState::orderFinished(std::unique_ptr<OrderState> &&current)
{
	return make_unique<OrderFinishedState>(m_order, std::move(current));
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

/// OrderFinishedState
OrderFinishedState::OrderFinishedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderFinishedState::reject(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At finish state, can not reject");
}

std::unique_ptr<OrderState> OrderFinishedState::receive(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

std::unique_ptr<OrderState> OrderFinishedState::startRepair(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At finish state, can not receive");
}

std::unique_ptr<OrderState> OrderFinishedState::endRepair(std::unique_ptr<OrderState> &&current, [[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At finish state, can not end repair");
}

std::unique_ptr<OrderState> OrderFinishedState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At finish state, can not pay");
}

std::unique_ptr<OrderState> OrderFinishedState::orderFinished(std::unique_ptr<OrderState> &&current)
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
	ThreadPool::getInstance().submit(&DataAccess::setOrderEvaluate, DataSource::getDataAccessInstance(), evaluate);
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

/// OrderRejectedState
OrderRejectedState::OrderRejectedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
		: OrderState(std::move(order), std::move(lastState))
{}

std::unique_ptr<OrderState> OrderRejectedState::receive(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At reject state, can not switch to received state");
}

std::unique_ptr<OrderState> OrderRejectedState::reject(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At reject state, can not switch to reject state");
}

std::unique_ptr<OrderState> OrderRejectedState::startRepair(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At reject state, can not switch to start repair state");
}

std::unique_ptr<OrderState> OrderRejectedState::endRepair(std::unique_ptr<OrderState> &&current, [[maybe_unused]] double transactionPrice)
{
	throw OrderNotAtRightState("At reject state, can not switch to end repair state");
}

std::unique_ptr<OrderState> OrderRejectedState::payTheOrder(std::unique_ptr<OrderState> &&current)
{
	throw OrderNotAtRightState("At reject state, can not pay");
}

std::unique_ptr<OrderState> OrderRejectedState::orderFinished(std::unique_ptr<OrderState> &&current)
{
	ThreadPool::getInstance().submit(&DataAccess::orderFinished, DataSource::getDataAccessInstance(), m_order.lock()->id());
	return make_unique<OrderFinishedState>(m_order, std::move(current));
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
