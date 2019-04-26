#ifndef HAR_ORDERENDREPAIRSTATEFACTORY_H
#define HAR_ORDERENDREPAIRSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

/// @brief The end repair state factory
class OrderEndRepairStateFactory : public OrderStateAbstractFactory {
public:
	~OrderEndRepairStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERENDREPAIRSTATEFACTORY_H
