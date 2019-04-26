#ifndef HAR_ORDERUNRECEIVEDSTATEFACTORY_H
#define HAR_ORDERUNRECEIVEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

struct OrderStateParameters;

/// @brief The unrecieved state factory
class OrderUnreceivedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderUnreceivedStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERUNRECEIVEDSTATEFACTORY_H
