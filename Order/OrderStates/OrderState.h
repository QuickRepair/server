#ifndef HAR_ORDERSTATE_H
#define HAR_ORDERSTATE_H

#include <chrono>
#include <memory>
#include "OrderEvaluate.h"
#include "AcceptableOrderPriceRange.h"

class Order;
class MerchantAccount;

class OrderState {
public:
	enum States { unreceivedState, receivedState, startRepairState, endRepairState, finishedState, rejectState};

	OrderState(std::weak_ptr<Order> order, std::chrono::system_clock::time_point timePoint);
	virtual ~OrderState() = 0;

	virtual void reject() = 0;
	virtual void receivedBy(std::weak_ptr<MerchantAccount> receiver) = 0;
	virtual void startRepair() = 0;
	virtual void endRepair(double transactionPrice) = 0;
	virtual void payTheOrder() = 0;
	virtual void orderFinished() = 0;

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
