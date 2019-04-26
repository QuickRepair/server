//
// Created by gaojian on 19-1-1.
//

#ifndef HAR_ORDERSTATEFACTORY_H
#define HAR_ORDERSTATEFACTORY_H

#include <memory>

class OrderState;
class Order;
struct OrderStateParameters;

/// @brief The abstract super class for state factories
class OrderStateAbstractFactory {
public:
	virtual ~OrderStateAbstractFactory() = 0;

	/// @brief Make a state
	/// @param order: make for the order
	/// @param parameters: the state specific details
	/// @return the created state
	virtual std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) = 0;
};

#endif //HAR_ORDERSTATEFACTORY_H
