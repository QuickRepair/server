#include "OrderUnreceivedStateFactory.h"
#include "../../Order/Order.h"
#include "../../Order/OrderStates/OrderUnreceivedState.h"
#include "../Database/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderUnreceivedStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto unreceived = make_shared<OrderUnreceivedState>(order, parameters.range, parameters.date);
	return unreceived;
}