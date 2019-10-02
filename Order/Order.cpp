#include "Order.h"
#include "OrderStates/OrderUnreceivedState.h"
#include "Account/CustomerAccount.h"
#include "Account/MerchantAccount.h"
#include "DataSource/DataAccess.h"
#include "DataSource/DataSource.hpp"
#include <stdexcept>

using std::make_unique;				using std::runtime_error;

Order::Order(unsigned long int id, CustomerAccount *committer, MerchantAccount *acceptor)
	: m_id{id}, m_committer{committer}, m_acceptor{acceptor}
{}

//Order::Order(unsigned long int id)
//	: m_id{id}
//{}

/*void Order::initOrderState()
{
	if(!m_currentState)
		m_currentState = make_unique<OrderUnreceivedState>(weak_from_this(), nullptr);
}*/

void Order::reject(MerchantAccount *receiver)
{
	if(receiver == m_acceptor)
		m_currentState = m_currentState->reject();
}

void Order::receive(MerchantAccount *receiver)
{
	if(receiver == m_acceptor)
		m_currentState = m_currentState->receive();
}

void Order::startRepair(MerchantAccount *receiver)
{
	m_currentState = m_currentState->startRepair();
}

void Order::endRepair(MerchantAccount *receiver, double transactionPrice)
{
	m_currentState = m_currentState->endRepair(transactionPrice);
}

void Order::pay(CustomerAccount *committer)
{
	m_currentState = m_currentState->payTheOrder();
}

void Order::finished()
{
	m_currentState = m_currentState->orderFinished();
}

AcceptableOrderPriceRange Order::priceRange()
{
	return m_currentState->priceRange();
}

double Order::transaction()
{
	return m_currentState->transaction();
}

void Order::setEvaluate(OrderEvaluate &evaluate)
{
	m_currentState->setEvaluate(evaluate);
}

OrderEvaluate Order::evaluate()
{
	return m_currentState->evaluate();
}

std::chrono::system_clock::time_point Order::rejectDate() const
{
	return m_currentState->rejectDate();
}

std::chrono::system_clock::time_point Order::createDate() const
{
	return m_currentState->createDate();
}

std::chrono::system_clock::time_point Order::receiveDate() const
{
	return m_currentState->receiveDate();
}

std::chrono::system_clock::time_point Order::startRepairDate() const
{
	return m_currentState->startRepairDate();
}

std::chrono::system_clock::time_point Order::endRepairDate() const
{
	return m_currentState->endRepairDate();
}

std::chrono::system_clock::time_point Order::finishDate() const
{
	return m_currentState->finishDate();
}

std::string Order::applianceType() const
{
	return DataSource::getDataAccessInstance()->getOrderApplianceTypeFor(m_id);
}

std::string Order::detail() const
{
	return DataSource::getDataAccessInstance()->getOrderDetailFor(m_id);
}

unsigned long int Order::id() const
{
	return m_id;
}

OrderState::States Order::currentState() const
{
	return m_currentState->atState();
}

/*bool Order::isNotReceived() const
{
	return currentState() == OrderState::States::unreceivedState;
}*/

/*unsigned long Order::committerId() const
{
	return m_committer.lock()->id();
}*/

unsigned long Order::receiverId() const
{
	return m_acceptor->id();
}
