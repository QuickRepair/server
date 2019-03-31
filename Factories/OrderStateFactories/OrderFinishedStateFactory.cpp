#include "OrderFinishedStateFactory.h"
#include "../../Order/Order.h"
#include "../../Order/OrderStates/OrderFinishedState.h"
#include "../Database/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderFinishedStateFactory::makeStateForOrder(std::weak_ptr<Order> order,
																		 OrderStateParameters &parameters)
{
	auto finished = make_shared<OrderFinishedState>(order, parameters.lastState);
	return finished;
}