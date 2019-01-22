#ifndef HAR_ORDERUNRECEIVEDSTATEFACTORY_H
#define HAR_ORDERUNRECEIVEDSTATEFACTORY_H

#include "orderstateabstractfactory.h"

class OrderStateParameters;

class OrderUnreceivedStateFactory : public OrderStateAbstractFactory {
public:
	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERUNRECEIVEDSTATEFACTORY_H
