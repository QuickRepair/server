#ifndef HAR_ORDER_H
#define HAR_ORDER_H

#include <string>
#include <memory>
#include <queue>
#include <chrono>
#include "Account/ContactInformation.h"
#include "OrderStates/AcceptableOrderPriceRange.h"
#include "OrderStates/OrderState.h"

class MerchantAccount;
class CustomerAccount;
class OrderEvaluate;

/// @brief Represent an order
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
	friend class OrderRejectedState;
	friend class OrderEndRepairStateFactory;
	friend class OrderFinishedStateFactory;
public:
	/// @brief A Constructor who make new order
	Order(unsigned long int id, std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
		  std::string applianceType, ContactInformation &contactWay, std::string detail);
	/// @brief A Constructor who load order
	Order(unsigned long int id, unsigned long committerId, unsigned long acceptorId,
			std::string applianceType, ContactInformation &contactWay, std::string detail);

	/// @brief Initial order state
	/// @note: MUST be called after constructed an order
	/// @param range: the acceptable price range
	void initOrderState(AcceptableOrderPriceRange &range);

	/// @brief Mark the order as rejected
	void reject();

	/// @brief Mark the order as received
	/// @param receiver: a weak_ptr to receiver
	void receivedBy(std::weak_ptr<MerchantAccount> receiver);

	/// @brief Mark the order as repairing
	void startRepair();

	/// @brief Mark the order as ended
	void endRepair(double transactionPrice);

	/// @brief Mark the order as payed
	void pay();

	/// @brief Mark the order as finished
	void finished();

	/// @brief Getters
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
	unsigned long committerId() const;
	unsigned long acceptorId() const;

private:
	/// @brief Set order current state
	/// @param state: a shared_ptr to state
	void setState(std::shared_ptr<OrderState> state);

	std::string m_applianceType;
	ContactInformation m_contactWay;
	std::string m_detail;
	unsigned long m_id;
	unsigned long m_committerId;
	unsigned long m_acceptorId;
	std::shared_ptr<OrderState> m_currentState;

	std::weak_ptr<CustomerAccount> m_committer;
	std::weak_ptr<MerchantAccount> m_acceptor;
};

#endif //HAR_ORDER_H
