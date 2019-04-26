#ifndef HAR_ORDERSTARTREPAIRSTATEFACTORY_H
#define HAR_ORDERSTARTREPAIRSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

/// @brief The start repair state factory
class OrderStartRepairStateFactory : public OrderStateAbstractFactory {
public:
	~OrderStartRepairStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERSTARTREPAIRSTATEFACTORY_H
