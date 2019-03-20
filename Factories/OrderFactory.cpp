#include "OrderFactory.h"
#include "OrderStateFactories/OrderStateAbstractFactory.h"
#include "Database/DatabaseConnection.hpp"
#include "Database/OrderStateParameters.h"
#include "../Order/Order.h"
#include "../Order/OrderStates/OrderState.h"
#include "../Account/ContactInformation.h"
#include "../Order/OrderStates/AcceptableOrderPriceRange.h"
#include <tuple>

using std::make_shared;			using std::get;
using std::shared_ptr;

std::shared_ptr<Order> OrderFactory::readOrder(std::weak_ptr<CustomerAccount> commiter, unsigned long orderId)
{
	auto orderInfo = DatabaseConnection::getInstance().queryOrderById(orderId);
	shared_ptr<Order> newOrder = make_shared<Order>(commiter, get<0>(orderInfo), get<1>(orderInfo), get<2>(orderInfo));

	newOrder->m_currentState = getStates(newOrder, get<3>(orderInfo));

	return newOrder;
}

std::shared_ptr<Order> OrderFactory::createOrder(std::weak_ptr<CustomerAccount> commiter, ContactInformation address, std::string detail, AcceptableOrderPriceRange range)
{
	//TODO get id from Database
	unsigned long id;
	shared_ptr<Order> newOrder = make_shared<Order>(commiter, address, detail, id);
	newOrder->orderInitState(range);
	return newOrder;
}

std::shared_ptr<OrderState> OrderFactory::getStates(shared_ptr<Order> &order, unsigned long lastStateId, unsigned call)
{
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters> stateInfo;
	if(call == 0)
		stateInfo = DatabaseConnection::getInstance().queryOrderStateByOrderIdAndStateId(order->id(), lastStateId);
	else
		stateInfo = DatabaseConnection::getInstance().queryOrderStateByOrderIdAndLastStateId(order->id(), lastStateId);

	shared_ptr<OrderStateAbstractFactory> stateFactory = get<0>(stateInfo);
	OrderStateParameters parameters = get<1>(stateInfo);
	if(parameters.lastStateId != 0)
		parameters.lastState = getStates(order, parameters.lastStateId);

	return stateFactory ? stateFactory->readStateForOrder(order, parameters) : nullptr;
}