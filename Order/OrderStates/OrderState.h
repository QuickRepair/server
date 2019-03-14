#ifndef HAR_ORDERSTATE_H
#define HAR_ORDERSTATE_H

#include <chrono>
#include <memory>
#include "OrderEvaluate.h"
#include "AcceptableOrderPriceRange.h"

class MerchantAccount;

class OrderState {
public:
	virtual ~OrderState() = 0;

	virtual void receivedBy(std::weak_ptr<MerchantAccount> receiver) = 0;
	virtual void startRepair() = 0;
	virtual void endRepair(double transactionPrice) = 0;
	virtual void orderFinished() = 0;

	virtual AcceptableOrderPriceRange priceRange() const = 0;
	virtual double transaction() const = 0;
	virtual void setEvaluate(OrderEvaluate &evaluate) = 0;
	virtual OrderEvaluate evaluate() const = 0;
	virtual std::chrono::system_clock::time_point date() const = 0;
};

#endif //HAR_ORDERSTATE_H
