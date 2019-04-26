#ifndef HAR_ORDERSTATECONSTRUCTORPARAMETERS_H
#define HAR_ORDERSTATECONSTRUCTORPARAMETERS_H

#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Order/OrderStates/OrderEvaluate.h"
#include <chrono>

class MerchantAccount;
class OrderState;

/// @brief A structure save order state specific values
struct OrderStateParameters {
	OrderStateParameters()
		: receiverId{0}, transactionPrice{0.0}, evaluate{OrderEvaluate()},
		lastStateId{0}, currentStateId{0}, receiver{nullptr}, lastState{nullptr}
	{}

	AcceptableOrderPriceRange range;
	unsigned long receiverId;
	double transactionPrice;
	OrderEvaluate evaluate;
	std::chrono::system_clock::time_point date;
	unsigned long lastStateId;
	unsigned long currentStateId;
	std::shared_ptr<MerchantAccount> receiver;
	std::shared_ptr<OrderState> lastState;
};

#endif //HAR_ORDERSTATECONSTRUCTORPARAMETERS_H
