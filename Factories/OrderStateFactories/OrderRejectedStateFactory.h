//
// Created by 17271 on 2019/5/11.
//

#ifndef HARSERVER_ORDERREJECTEDSTATEFACTORY_H
#define HARSERVER_ORDERREJECTEDSTATEFACTORY_H

#include "OrderStateAbstractFactory.h"

class OrderRejectedStateFactory : public OrderStateAbstractFactory {
public:
	~OrderRejectedStateFactory() override = default;

	/// @override
	std::shared_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters) override;
};


#endif //HARSERVER_ORDERREJECTEDSTATEFACTORY_H
