#ifndef HAR_ORDERSTATECONSTRUCTORPARAMETERS_H
#define HAR_ORDERSTATECONSTRUCTORPARAMETERS_H

#include "../entity/order/orderpricerange.h"
#include "../entity/order/evaluate.h"
#include <chrono>

class MerchantAccount;
class OrderState;

struct OrderStateParameters {
	OrderStateParameters()
		: receiverId{0}, transactionPrice{0.0}, evaluate{Evaluate()},
		lastStateId{0}, currentStateId{0}, receiver{nullptr}, lastState{nullptr}
	{}

	OrderPriceRange range;
	unsigned long receiverId;
	double transactionPrice;
	Evaluate evaluate;
	std::chrono::system_clock::time_point date;
	unsigned long lastStateId;
	unsigned long currentStateId;
	std::shared_ptr<MerchantAccount> receiver;
	std::shared_ptr<OrderState> lastState;
};

#endif //HAR_ORDERSTATECONSTRUCTORPARAMETERS_H
