#ifndef HAR_ORDERFINISHEDSTATEFACTORY_H
#define HAR_ORDERFINISHEDSTATEFACTORY_H

#include "orderstateabstractfactory.h"

class OrderFinishedStateFactory : public OrderStateAbstractFactory {
public:
	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERFINISHEDSTATEFACTORY_H
