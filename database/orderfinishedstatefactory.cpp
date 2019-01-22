#include "orderfinishedstatefactory.h"
#include "../entity/order/order.h"
#include "../entity/order/orderfinishedstate.h"
#include "orderstateparameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderFinishedStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto finished = make_shared<OrderFinishedState>(order, parameters.lastState);
	order.lock()->m_finishedState = finished;
	return finished;
}