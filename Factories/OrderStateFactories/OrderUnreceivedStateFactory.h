#ifndef HAR_ORDERUNRECEIVEDSTATEFACTORY_H
#define HAR_ORDERUNRECEIVEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

class OrderStateParameters;

class OrderUnreceivedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderUnreceivedStateFactory() override = default;

	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERUNRECEIVEDSTATEFACTORY_H
