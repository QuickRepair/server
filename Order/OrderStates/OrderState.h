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
	enum class States {
		unreceivedState,
		receivedState,
		startRepairState,
		endRepairState,
		finishedState,
		rejectState
	};

	explicit OrderState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState = nullptr)
		: m_order{order}, m_lastState{std::move(lastState)}
	{}
	virtual ~OrderState() = 0;

	/// @brief Transform state to rejected
	virtual std::unique_ptr<OrderState> reject() = 0;

	/// @brief Transform state to received
	/// @param receiver: a weak_ptr to receiver
	virtual std::unique_ptr<OrderState> receive() = 0;

	/// @brief Transform state to repairing
	virtual std::unique_ptr<OrderState> startRepair() = 0;

	/// @brief Transform state to ended
	virtual std::unique_ptr<OrderState> endRepair(double transactionPrice) = 0;

	/// @brief Transform state to payed
	virtual std::unique_ptr<OrderState> payTheOrder() = 0;

	/// @brief Transform state to finished
	virtual std::unique_ptr<OrderState> orderFinished() = 0;

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
	std::weak_ptr<Order> m_order;
	std::unique_ptr<OrderState> m_lastState;
};

#endif //HAR_ORDERSTATE_H
