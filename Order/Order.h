#ifndef HAR_ORDER_H
#define HAR_ORDER_H

#include <string>
#include <memory>
#include <queue>
#include <chrono>
#include "../Account/ContactInformation.h"
#include "OrderStates/AcceptableOrderPriceRange.h"

class OrderState;
class MerchantAccount;
class CustomerAccount;
class OrderEvaluate;

class Order : public std::enable_shared_from_this<Order> {
	friend class OrderUnreceivedState;
	friend class OrderReceivedState;
	friend class OrderStartRepairState;
	friend class OrderEndRepairState;
	friend class OrderFinishedState;
	friend class OrderFactory;
	friend class OrderUnreceivedStateFactory;
	friend class OrderReceivedStateFactory;
	friend class OrderStartRepairStateFactory;
	friend class OrderEndRepairStateFactory;
	friend class OrderFinishedStateFactory;
public:
	Order(std::weak_ptr<CustomerAccount> commiter, ContactInformation contactWay, std::string detail, unsigned long int id);

	void receivedBy(std::weak_ptr<MerchantAccount> receiver);
	void startRepair();
	void endRepair(double transactionPrice);
	void orderFinished();
	AcceptableOrderPriceRange priceRange();
	double transaction();
	void setEvaluate(OrderEvaluate &evaluate);
	OrderEvaluate evaluate();
	std::chrono::system_clock::time_point createDate();
	std::chrono::system_clock::time_point receiveDate();
	std::chrono::system_clock::time_point startRepairDate();
	std::chrono::system_clock::time_point endRepairDate();

	unsigned long int id() const;
	bool isNotReceived() const;

private:
	void orderInitState(AcceptableOrderPriceRange range);
	void setState(std::shared_ptr<OrderState> state);

	std::shared_ptr<OrderState> m_unreceivedState;
	std::shared_ptr<OrderState> m_receivedState;
	std::shared_ptr<OrderState> m_startRepairState;
	std::shared_ptr<OrderState> m_endRepairState;
	std::shared_ptr<OrderState> m_finishedState;

	ContactInformation m_contactWay;
	std::string m_detail;
	unsigned long int m_id;
	std::shared_ptr<OrderState> m_currentState;

	std::weak_ptr<CustomerAccount> m_committer;
	std::weak_ptr<MerchantAccount> m_acceptor;
};

#endif //HAR_ORDER_H
