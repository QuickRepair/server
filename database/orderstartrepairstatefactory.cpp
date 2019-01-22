#include "orderstartrepairstatefactory.h"
#include "../entity/order/order.h"
#include "../entity/order/orderstartrepairstate.h"
#include "orderstateparameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderStartRepairStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto startRepair = make_shared<OrderStartRepairState>(order, parameters.lastState, parameters.date);
	order.lock()->m_startRepairState = startRepair;
	return startRepair;
}