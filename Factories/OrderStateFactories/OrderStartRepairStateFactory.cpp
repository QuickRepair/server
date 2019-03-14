#include "OrderStartRepairStateFactory.h"
#include "../../Order/Order.h"
#include "../../Order/OrderStates/OrderStartRepairState.h"
#include "../Database/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderStartRepairStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto startRepair = make_shared<OrderStartRepairState>(order, parameters.lastState, parameters.date);
	order.lock()->m_startRepairState = startRepair;
	return startRepair;
}