#ifndef HAR_ORDERENDREPAIRSTATEFACTORY_H
#define HAR_ORDERENDREPAIRSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

class OrderEndRepairStateFactory : public OrderStateAbstractFactory {
public:
	~OrderEndRepairStateFactory() override = default;

	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERENDREPAIRSTATEFACTORY_H
