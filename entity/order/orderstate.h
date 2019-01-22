#ifndef HAR_ORDERSTATE_H
#define HAR_ORDERSTATE_H

#include <chrono>
#include <memory>
#include "evaluate.h"
#include "orderpricerange.h"

class MerchantAccount;

class OrderState {
public:
	virtual void receivedBy(std::weak_ptr<MerchantAccount> receiver) = 0;
	virtual void startRepair() = 0;
	virtual void endRepair(double transactionPrice) = 0;
	virtual void orderFinished() = 0;

	virtual OrderPriceRange priceRange() const = 0;
	virtual double transaction() const = 0;
	virtual void setEvaluate(Evaluate &evaluate) = 0;
	virtual Evaluate evaluate() const = 0;
	virtual std::chrono::system_clock::time_point date() const = 0;
};

#endif //HAR_ORDERSTATE_H
