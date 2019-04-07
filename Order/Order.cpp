#include "Order.h"
#include "OrderStates/OrderUnreceivedState.h"
#include "Factories/OrderStateFactories/OrderStateAbstractFactory.h"
#include <stdexcept>

using std::make_shared;				using std::runtime_error;

Order::Order(unsigned long int id, std::weak_ptr<CustomerAccount> committer,
			 std::string applianceType, ContactInformation &contactWay, std::string detail)
	: m_applianceType{applianceType},
	m_contactWay{contactWay},
	m_detail{detail}, m_id{id},
	m_committer{committer}
{}

Order::Order(unsigned long int id, std::weak_ptr<CustomerAccount> committer,
	  std::string applianceType, ContactInformation &contactWay, std::string detail, std::shared_ptr<OrderState> currentState)
		: m_applianceType{applianceType},
		  m_contactWay{contactWay},
		  m_detail{detail}, m_id{id},
		  m_committer{committer}, m_currentState{currentState}
{}

void Order::initOrderState(AcceptableOrderPriceRange &range)
{
	if(!m_currentState)
		m_currentState = make_shared<OrderUnreceivedState>(weak_from_this(), range);
}

void Order::reject()
{
	m_currentState->reject();
}

void Order::receivedBy(std::weak_ptr<MerchantAccount> receiver)
{
	m_currentState->receivedBy(std::move(receiver));
}

void Order::startRepair()
{
	m_currentState->startRepair();
}

void Order::endRepair(double transactionPrice)
{
	m_currentState->endRepair(transactionPrice);
}

void Order::pay()
{
	m_currentState->payTheOrder();
}

void Order::finished()
{
	m_currentState->orderFinished();
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
	return m_applianceType;
}

std::string Order::detail() const
{
	return m_detail;
}

unsigned long int Order::id() const
{
	return m_id;
}

OrderState::States Order::currentState() const
{
	return m_currentState->atState();
}

bool Order::isNotReceived() const
{
	return m_acceptor.lock() == nullptr;
}

void Order::setState(std::shared_ptr<OrderState> state)
{
	m_currentState = state;
}