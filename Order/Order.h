#ifndef HAR_ORDER_H
#define HAR_ORDER_H

#include <string>
#include <memory>
#include <queue>
#include <chrono>
#include "../Account/ContactInformation.h"
#include "OrderStates/AcceptableOrderPriceRange.h"
#include "OrderStates/OrderState.h"

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
	Order(unsigned long int id, std::weak_ptr<CustomerAccount> committer,
		  std::string applianceType, ContactInformation contactWay, std::string detail);

	void reject();
	void receivedBy(std::weak_ptr<MerchantAccount> receiver);
	void startRepair();
	void endRepair(double transactionPrice);
	void finished();

	AcceptableOrderPriceRange priceRange();
	double transaction();
	void setEvaluate(OrderEvaluate &evaluate);
	OrderEvaluate evaluate();
	std::chrono::system_clock::time_point rejectDate() const;
	std::chrono::system_clock::time_point createDate() const;
	std::chrono::system_clock::time_point receiveDate() const;
	std::chrono::system_clock::time_point startRepairDate() const;
	std::chrono::system_clock::time_point endRepairDate() const;
	std::chrono::system_clock::time_point finishDate() const;

	std::string applianceType() const;
	std::string detail() const;
	unsigned long int id() const;
	OrderState::States currentState() const;
	bool isNotReceived() const;

private:
	void orderInitState(AcceptableOrderPriceRange range);
	void setState(std::shared_ptr<OrderState> state);

	std::string m_applianceType;
	ContactInformation m_contactWay;
	std::string m_detail;
	unsigned long int m_id;
	std::shared_ptr<OrderState> m_currentState;

	std::weak_ptr<CustomerAccount> m_committer;
	std::weak_ptr<MerchantAccount> m_acceptor;
};

#endif //HAR_ORDER_H
