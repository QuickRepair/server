#include "OrderEndRepairStateFactory.h"
#include "../../Order/Order.h"
#include "../../Order/OrderStates/OrderEndRepairState.h"
#include "../Database/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderEndRepairStateFactory::makeStateForOrder(std::weak_ptr<Order> order,
																		  OrderStateParameters &parameters)
{
	auto endRepair = make_shared<OrderEndRepairState>(order, parameters.lastState, parameters.transactionPrice, parameters.evaluate, parameters.date);
	return endRepair;
}
