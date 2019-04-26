#ifndef HAR_ORDERFINISHEDSTATEFACTORY_H
#define HAR_ORDERFINISHEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

/// @brief The finished state factory
class OrderFinishedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderFinishedStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERFINISHEDSTATEFACTORY_H
