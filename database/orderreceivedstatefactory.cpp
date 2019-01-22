#include "orderreceivedstatefactory.h"
#include "../entity/order/order.h"
#include "../entity/order/orderreceivedstate.h"
#include "orderstateparameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderReceivedStateFactory::readStateForOrder(std::weak_ptr<Order> order, OrderStateParameters &parameters)
{
	auto received = make_shared<OrderReceivedState>(order, parameters.lastState, parameters.receiver, parameters.date);
	order.lock()->m_receivedState = received;
	return received;
}