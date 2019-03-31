#ifndef HAR_ORDERRECEIVEDSTATEFACTORY_H
#define HAR_ORDERRECEIVEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

class OrderReceivedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderReceivedStateFactory() override = default;

	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERRECEIVEDSTATEFACTORY_H
