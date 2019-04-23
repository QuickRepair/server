//
// Created by gaojian on 19-1-1.
//

#ifndef HAR_ORDERSTATEFACTORY_H
#define HAR_ORDERSTATEFACTORY_H

#include <memory>

class OrderState;
class Order;
struct OrderStateParameters;

class OrderStateAbstractFactory {
public:
	virtual ~OrderStateAbstractFactory() = 0;

	virtual std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) = 0;
};

#endif //HAR_ORDERSTATEFACTORY_H
