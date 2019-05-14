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
		receiver{nullptr}, lastState{nullptr}
	{}

	AcceptableOrderPriceRange range;
	unsigned long receiverId;
	double transactionPrice;
	OrderEvaluate evaluate;
	std::chrono::system_clock::time_point create_date;
	std::chrono::system_clock::time_point received_date;
	std::chrono::system_clock::time_point start_repair_date;
	std::chrono::system_clock::time_point end_repair_date;
	std::chrono::system_clock::time_point finish_date;
	std::chrono::system_clock::time_point reject_date;
	std::shared_ptr<MerchantAccount> receiver;
	std::shared_ptr<OrderState> lastState;
};

#endif //HAR_ORDERSTATECONSTRUCTORPARAMETERS_H
