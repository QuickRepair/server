
#ifndef HAR_ORDERFACTORY_H
#define HAR_ORDERFACTORY_H

#include <memory>
#include <string>
#include <list>
#include "Order/OrderStates/OrderState.h"
#include "types.h"

class Order;

/// @brief The order factory, make or load orders from this class
class OrderFactory {
public:
	std::shared_ptr<Order> loadOrder(OrderId id);
	std::list<std::shared_ptr<Order>> loadAccountOrders(AccountId id);

	/// @brief Read order list for account
	/// @param account: a weak_ptr to account
	/// @return a list of shared_ptr to orders belong to the account
//	std::list<std::shared_ptr<Order>> getOrdersListForAccount(std::weak_ptr<Account> account);

	/// @brief Create a new order
	/// @param committer: the order committer
	/// @param acceptor: the specified order acceptor
	/// @param applianceType: type of the appliance in the order
	///	@param contactWay: how to contact committer
	/// @param detail: detail description of the order
	/// @param range: range of acceptable price
	/// @return new order
//	std::shared_ptr<Order> createOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
//									   std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range);

	/// @brief Persistence last order state
	/// @param order: a weak_ptr to order
//	void persistenceOrderState(std::weak_ptr<Order> &order);

private:
	/// @brief Make states for order
	/// @param order: make for the order
	/// @return a shared_ptr to OrderState
	std::unique_ptr<OrderState> loadOrderStates(std::shared_ptr<Order> &order);
};

template <typename T>
class OrderStateFactory {
public:
	/// @brief Make a state
	/// @param order: make for the order
	/// @param parameters: the state specific details
	/// @return the created state
	std::unique_ptr<OrderState> makeStateForOrder(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState)
	{
		return std::make_unique<T>(order, std::move(lastState));
	}
};

#endif //HAR_ORDERFACTORY_H
