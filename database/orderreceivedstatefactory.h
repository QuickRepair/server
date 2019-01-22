#ifndef HAR_ORDERRECEIVEDSTATEFACTORY_H
#define HAR_ORDERRECEIVEDSTATEFACTORY_H

#include "orderstateabstractfactory.h"

class OrderReceivedStateFactory : public OrderStateAbstractFactory {
public:
	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERRECEIVEDSTATEFACTORY_H
