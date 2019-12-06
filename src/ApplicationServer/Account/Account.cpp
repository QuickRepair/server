#include "Account.h"
#include "DataSource/DataSource.hpp"
#include "Order/Order.h"
#include "Thread/ThreadPool.hpp"

using std::string;					using std::make_unique;

Account::Account(AccountId id)
    : m_id{id}/*, m_account{std::move(account)}, m_password{std::move(password)}*/
{}

AccountId Account::id() const {
    return m_id;
}

const string Account::password() const {
//    return m_password;
	return DataSource::getDataAccessInstance()->getPasswordFor(m_id);
}

void Account::setPassword(const string &password) {
	ThreadPool::getInstance().submit(&DataAccess::setPasswordFor, DataSource::getDataAccessInstance(), m_id, password);
//    m_password = password;
}

const string Account::account() const {
	return DataSource::getDataAccessInstance()->getAccountFor(m_id);
//    return m_account;
}

void Account::setAccount(const string &userName) {
	ThreadPool::getInstance().submit(&DataAccess::setAccountFor, DataSource::getDataAccessInstance(), m_id, userName);
//    m_account = userName;
}

using std::weak_ptr;						using std::list;

CustomerAccount::CustomerAccount(AccountId id)
		:Account{id}
{}

void CustomerAccount::iAmPublishAnOrder(std::weak_ptr<Order> order)
{
	m_orders[order.lock()->id()] = order.lock();
}

std::list<std::weak_ptr<Order>> CustomerAccount::getOrderList() const
{
	std::list<std::weak_ptr<Order>> orderList;
	for (auto &o : m_orders)
		orderList.emplace_back(o.second);
	return orderList;
}

/*
void CustomerAccount::cancelOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->finished();
}

void CustomerAccount::evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate)
{
	if(isMyOrder(order))
		order.lock()->setEvaluate(evaluate);
}

void CustomerAccount::payTheOrder(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->pay();
}

bool CustomerAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return m_orders.end() != std::find_if(m_orders.begin(), m_orders.end(),
			[&order](weak_ptr<Order> a){ return a.lock()->id() == order.lock()->id(); });
}

std::list<std::weak_ptr<Order>> CustomerAccount::myOrdersList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_orders)
		orderList.push_back(order);
	return orderList;
}

void CustomerAccount::loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info)
{
	m_contactInfo.clear();
	for(auto &element : info)
		m_contactInfo.push_back(element);
}

void CustomerAccount::loadOrder(std::shared_ptr<Order> order)
{
	if(!isMyOrder(order))
		m_orders.push_back(order);
}*/

MerchantAccount::MerchantAccount(AccountId id)
		:CustomerAccount{id}
{
	m_serviceType = make_unique<MerchantService>(this);
}

void MerchantAccount::orderWaitToBeAccept(std::weak_ptr<Order> order)
{
	m_orders[order.lock()->id()] = order.lock();
}

/*void MerchantAccount::acceptOrder(std::weak_ptr<Order> order)
{
	if(isMyUnreceivedOrder(order) && order.lock()->isNotReceived())
	{
		order.lock()->receivedBy(weak_from_this());
		m_processedOrders.push_back(order.lock());
		m_unreceivedOrders.remove(order.lock());
	}
}

void MerchantAccount::startRepair(std::weak_ptr<Order> order)
{
	if(isMyOrder(order))
		order.lock()->startRepair();
}

void MerchantAccount::endRepair(std::weak_ptr<Order> order, double transaction)
{
	if(isMyOrder(order))
		order.lock()->endRepair(transaction);
}

void MerchantAccount::rejectOrder(std::weak_ptr<Order> order)
{
	if(isMyUnreceivedOrder(order))
	{
		order.lock()->reject();
		m_unreceivedOrders.remove(order.lock());
		m_processedOrders.push_back(order.lock());
	}
}

bool MerchantAccount::isMyUnreceivedOrder(std::weak_ptr<Order> order) const
{
	return m_unreceivedOrders.end() != std::find_if(m_unreceivedOrders.begin(), m_unreceivedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
}

bool MerchantAccount::isMyOrder(std::weak_ptr<Order> order) const
{
	return isMySubmittedOrder(order) || isMyUnreceivedOrder(order) || m_processedOrders.end() != std::find_if(m_processedOrders.begin(), m_processedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
}

bool MerchantAccount::isMySubmittedOrder(std::weak_ptr<Order> order) const
{
	return CustomerAccount::isMyOrder(order);
}

bool MerchantAccount::isMyProcessedOrder(std::weak_ptr<Order> order) const
{
	return m_processedOrders.end() != std::find_if(m_processedOrders.begin(), m_processedOrders.end(),
			[&order](shared_ptr<Order> a){ return a->id() == order.lock()->id(); });
}

std::list<std::weak_ptr<Order>> MerchantAccount::myUnreceivedOrderList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_unreceivedOrders)
		orderList.push_back(order);
	return orderList;
}

std::list<std::weak_ptr<Order>> MerchantAccount::myOrdersList() const
{
	list<weak_ptr<Order>> orderList;
	for(auto &order : m_processedOrders)
		orderList.push_back(order);
	for(auto &order : m_unreceivedOrders)
		orderList.push_back(order);
	return orderList;
}*/

MerchantService *MerchantAccount::getService()
{
	return m_serviceType.get();
}

/*
void MerchantAccount::updateSupportedService(std::list<std::string> appliancType, double maxDistance)
{
	m_serviceType->setMaxRepairDistance(maxDistance);
	m_serviceType->setSupportApplianceType(appliancType);
}

void MerchantAccount::loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info)
{
	m_contactInfo.clear();
	for(auto &element : info)
		m_contactInfo.push_back(element);
}

void MerchantAccount::loadOrder(std::shared_ptr<Order> order)
{
	if(!isMyOrder(order))
	{
		if (order->currentState() == OrderState::States::unreceivedState)
			m_unreceivedOrders.push_back(order);
		else if(order->acceptorId() == id())
			m_processedOrders.push_back(order);
		else
			CustomerAccount::loadOrder(order);
	}
}

void MerchantAccount::loadServiceType(std::unique_ptr<MerchantService> &&service)
{
	m_serviceType = std::move(service);
}*/
