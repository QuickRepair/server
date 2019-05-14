#include "OrderStartRepairStateFactory.h"
#include "Order/Order.h"
#include "Order/OrderStates/OrderStartRepairState.h"
#include "Factories/DataSource/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderStartRepairStateFactory::makeStateForOrder(std::weak_ptr<Order> order,
																			OrderStateParameters &parameters)
{
	auto startRepair = make_shared<OrderStartRepairState>(order, parameters.lastState, parameters.start_repair_date);
	return startRepair;
}