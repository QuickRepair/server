#ifndef HAR_ORDERENDREPAIRSTATEFACTORY_H
#define HAR_ORDERENDREPAIRSTATEFACTORY_H

#include "orderstateabstractfactory.h"

class OrderEndRepairStateFactory : public OrderStateAbstractFactory {
public:
	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERENDREPAIRSTATEFACTORY_H
