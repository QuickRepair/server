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
	virtual std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) = 0;

	/// @brief Transform state to received
	/// @param receiver: a weak_ptr to receiver
	virtual std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) = 0;

	/// @brief Transform state to repairing
	virtual std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) = 0;

	/// @brief Transform state to ended
	virtual std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) = 0;

	/// @brief Transform state to payed
	virtual std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) = 0;

	/// @brief Transform state to finished
	virtual std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) = 0;

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

class OrderUnreceivedState : public OrderState {
public:
	OrderUnreceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderUnreceivedState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

class OrderReceivedState : public OrderState {
public:
	OrderReceivedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderReceivedState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

class OrderStartRepairState : public OrderState {
public:
	OrderStartRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderStartRepairState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

class OrderEndRepairState : public OrderState {
public:
	OrderEndRepairState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderEndRepairState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

class OrderFinishedState : public OrderState {
public:
	OrderFinishedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderFinishedState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

class OrderRejectedState : public OrderState {
public:
	OrderRejectedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderRejectedState() override = default;

	std::unique_ptr<OrderState> reject(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> receive(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> startRepair(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> endRepair(std::unique_ptr<OrderState> &&current, double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder(std::unique_ptr<OrderState> &&current) override;
	std::unique_ptr<OrderState> orderFinished(std::unique_ptr<OrderState> &&current) override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;
};

#endif //HAR_ORDERSTATE_H
