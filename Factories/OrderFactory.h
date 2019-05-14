
#ifndef HAR_ORDERFACTORY_H
#define HAR_ORDERFACTORY_H

#include <memory>
#include <string>
#include <list>

class Order;
class OrderState;
class Account;
class CustomerAccount;
class MerchantAccount;
class ContactInformation;
class AcceptableOrderPriceRange;

/// @brief The order factory, make or load orders from this class
class OrderFactory {
public:
	/// @brief Read order list for account
	/// @param account: a weak_ptr to account
	/// @return a list of shared_ptr to orders belong to the account
	std::list<std::shared_ptr<Order>> getOrdersListForAccount(std::weak_ptr<Account> account);

	/// @brief Create a new order
	/// @param committer: the order committer
	/// @param acceptor: the specified order acceptor
	/// @param applianceType: type of the appliance in the order
	///	@param contactWay: how to contact committer
	/// @param detail: detail description of the order
	/// @param range: range of acceptable price
	/// @return new order
	std::shared_ptr<Order> createOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
									   std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range);

	/// @brief Persistence last order state
	/// @param order: a weak_ptr to order
	void persistenceOrderState(std::weak_ptr<Order> &order);

private:
	/// @brief Make states for order
	/// @param order: make for the order
	/// @return a shared_ptr to OrderState
	std::shared_ptr<OrderState> getStates(std::shared_ptr<Order> &order);

};

#endif //HAR_ORDERFACTORY_H
