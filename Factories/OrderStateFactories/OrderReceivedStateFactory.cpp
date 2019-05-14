#include "OrderReceivedStateFactory.h"
#include "Order/Order.h"
#include "Order/OrderStates/OrderReceivedState.h"
#include "Factories/DataSource/OrderStateParameters.h"

using std::make_shared;

std::shared_ptr<OrderState> OrderReceivedStateFactory::makeStateForOrder(std::weak_ptr<Order> order,
																		 OrderStateParameters &parameters)
{
	auto received = make_shared<OrderReceivedState>(order, parameters.lastState, parameters.receiver, parameters.received_date);
	return received;
}