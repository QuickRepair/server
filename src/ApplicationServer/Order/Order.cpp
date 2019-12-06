#include "Order.h"
#include "Order/OrderStates/OrderState.h"
#include "Account/Account.h"
#include "DataSource/DataSource.hpp"
#include <stdexcept>

using std::make_unique;				using std::runtime_error;

Order::Order(OrderId id, AssociatedAccount committer, AssociatedAccount acceptor)
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
	if(receiver->id() == m_acceptor.id())
		m_currentState = m_currentState->reject(std::move(m_currentState));
}

void Order::receive(MerchantAccount *receiver)
{
	if(receiver->id() == m_acceptor.id())
		m_currentState = m_currentState->receive(std::move(m_currentState));
}

void Order::startRepair(MerchantAccount *receiver)
{
	if(receiver->id() == m_acceptor.id())
		m_currentState = m_currentState->startRepair(std::move(m_currentState));
}

void Order::endRepair(MerchantAccount *receiver, double transactionPrice)
{
	if (receiver->id() == m_acceptor.id())
		m_currentState = m_currentState->endRepair(std::move(m_currentState), transactionPrice);
}

void Order::pay(CustomerAccount *committer)
{
	if (committer->id() == m_committer.id())
		m_currentState = m_currentState->payTheOrder(std::move(m_currentState));
}

void Order::finished()
{
	m_currentState = m_currentState->orderFinished(std::move(m_currentState));
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

OrderId Order::id() const
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

AccountId Order::receiverId() const
{
	return m_acceptor.id();
}

AssociatedAccount::AssociatedAccount(AccountId id)
	: type{Type::id}
{
	account.m_id = id;
}

AssociatedAccount::AssociatedAccount(Account *ptr)
	: type{Type::ptr}
{
	account._ptr = ptr;
}

void AssociatedAccount::assign(AccountId id)
{
	type = Type::id;
	account.m_id = id;
}

void AssociatedAccount::assign(Account *ptr)
{
	type = Type::ptr;
	account._ptr = ptr;
}

AccountId AssociatedAccount::id() const
{
	AccountId retId;
	switch(type)
	{
		case Type::id:
			retId = account.m_id;
			break;
		case Type::ptr:
			retId = account._ptr->id();
			break;
	}
	return retId;
}

Account* AssociatedAccount::ptr() const
{
	Account *retAccount;
	switch (type)
	{
		case Type::id:
			retAccount = nullptr;
			break;
		case Type::ptr:
			retAccount = account._ptr;
			break;
	}
	return retAccount;
}
