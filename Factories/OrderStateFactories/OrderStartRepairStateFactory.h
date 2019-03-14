#ifndef HAR_ORDERSTARTREPAIRSTATEFACTORY_H
#define HAR_ORDERSTARTREPAIRSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

class OrderStartRepairStateFactory : public OrderStateAbstractFactory {
public:
	~OrderStartRepairStateFactory() override = default;

	std::shared_ptr<OrderState> readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERSTARTREPAIRSTATEFACTORY_H
