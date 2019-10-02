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
	friend class OrderFactory;
public:
	/// @brief A Constructor who make new order
	Order(unsigned long int id, CustomerAccount *committer, MerchantAccount *acceptor);
	/// @brief A Constructor who load order
//	Order(unsigned long int id, unsigned long committerId, unsigned long acceptorId,
//			std::string applianceType, ContactInformation &contactWay, std::string detail);

	/// @brief Initial order state
	/// @note: MUST be called after constructed an order
	/// @param range: the acceptable price range
//	void initOrderState();

	/// @brief Mark the order as rejected
	void reject(MerchantAccount *receiver);

	/// @brief Mark the order as received
	/// @param receiver: a weak_ptr to receiver
	void receive(MerchantAccount *receiver);

	/// @brief Mark the order as repairing
	void startRepair(MerchantAccount *receiver);

	/// @brief Mark the order as ended
	void endRepair(MerchantAccount *receiver, double transactionPrice);

	/// @brief Mark the order as payed
	void pay(CustomerAccount *committer);

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
//	bool isNotReceived() const;
//	unsigned long committerId() const;
	unsigned long receiverId() const;

private:
	unsigned long m_id;
	std::unique_ptr<OrderState> m_currentState;

	CustomerAccount *m_committer;
	MerchantAccount *m_acceptor;
};

#endif //HAR_ORDER_H
