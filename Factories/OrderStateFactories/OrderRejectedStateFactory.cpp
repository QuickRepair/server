//
// Created by 17271 on 2019/5/11.
//

#include "Order/OrderStates/OrderRejectedState.h"
#include "OrderRejectedStateFactory.h"
#include "Factories/DataSource/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderRejectedStateFactory::makeStateForOrder(std::weak_ptr<Order> order,
																		 OrderStateParameters &parameters)
{
	return make_shared<OrderRejectedState>(order, parameters.lastState, parameters.date);
}
