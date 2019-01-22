
#include "orderunreceivedstatefactory.h"
#include "../entity/order/order.h"
#include "../entity/order/orderunreceivedstate.h"
#include "orderstateparameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderUnreceivedStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto unreceived = make_shared<OrderUnreceivedState>(order, parameters.range, parameters.date);
	order.lock()->m_unreceivedState = unreceived;
	return unreceived;
}