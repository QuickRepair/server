#ifndef HAR_ORDERRECEIVEDSTATEFACTORY_H
#define HAR_ORDERRECEIVEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

/// @brief The received state factory
class OrderReceivedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderReceivedStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};

#endif //HAR_ORDERRECEIVEDSTATEFACTORY_H
