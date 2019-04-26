#ifndef HAR_ORDERSTATE_H
#define HAR_ORDERSTATE_H

#include <chrono>
#include <memory>
#include "OrderEvaluate.h"
#include "AcceptableOrderPriceRange.h"

class Order;
class MerchantAccount;

/// @brief An interface of order states
class OrderState {
public:
	/// @brief All list all states
	enum States { unreceivedState, receivedState, startRepairState, endRepairState, finishedState, rejectState};

	OrderState(std::weak_ptr<Order> order, std::chrono::system_clock::time_point timePoint);
	virtual ~OrderState() = 0;

	/// @brief Transform state to rejected
	virtual void reject() = 0;

	/// @brief Transform state to received
	/// @param receiver: a weak_ptr to receiver
	virtual void receivedBy(std::weak_ptr<MerchantAccount> receiver) = 0;

	/// @brief Transform state to repairing
	virtual void startRepair() = 0;

	/// @brief Transform state to ended
	virtual void endRepair(double transactionPrice) = 0;

	/// @brief Transform state to payed
	virtual void payTheOrder() = 0;

	/// @brief Transform state to finished
	virtual void orderFinished() = 0;

	/// @brief Getters
	virtual AcceptableOrderPriceRange priceRange() const = 0;
	virtual double transaction() const = 0;
	virtual void setEvaluate(OrderEvaluate &evaluate) = 0;
	virtual OrderEvaluate evaluate() const = 0;
	virtual States atState() const = 0;
	virtual std::chrono::system_clock::time_point createDate() const = 0;
	virtual std::chrono::system_clock::time_point rejectDate() const = 0;
	virtual std::chrono::system_clock::time_point receiveDate() const = 0;
	virtual std::chrono::system_clock::time_point startRepairDate() const = 0;
	virtual std::chrono::system_clock::time_point endRepairDate() const = 0;
	virtual std::chrono::system_clock::time_point finishDate() const = 0;

protected:
	std::chrono::system_clock::time_point m_stateChangeDate;
	std::weak_ptr<Order> m_order;
};

#endif //HAR_ORDERSTATE_H
