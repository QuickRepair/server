//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ORDERMANAGERSUBJECT_H
#define HARSERVER_ORDERMANAGERSUBJECT_H

#include <memory>
#include <list>
#include <unordered_map>
#include "types.h"

class Order;

/// @brief The interface of OrderManager
/// @note: use subclass OrderManagerProxy instead of OrderManager
class OrderManagerSubject {
public:
	virtual ~OrderManagerSubject() = 0;

	virtual std::shared_ptr<Order> createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType, std::string detail) = 0;

	virtual std::weak_ptr<Order> getOrder(OrderId id) = 0;
	virtual std::unordered_map<OrderId, std::shared_ptr<Order>> loadAccountOrders(AccountId id) = 0;
	/// @brief Publish new an order
	/// @param committer: a weak_ptr to customer
	/// @param acceptor: a weak_ptr to merchant
	/// @param applianceType: the type or appliance in string
	/// @param contactWay: how to contact
	/// @param detail: detail description for order
	/// @param range: acceptable price range for the order
	/// @return new order just published
//	virtual std::weak_ptr<Order> publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
//					  std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) = 0;

	/// @brief Accept an order
	/// @param acceptor: a weak_ptr to receiving merchant
	/// @param order: a weak_ptr to order
//	virtual void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) = 0;

	/// @brief Reject an order
	/// @param account: a weak_ptr to rejecting merchant
	/// @param order: a weak_ptr to order
//	virtual void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) = 0;

	/// @brief Start repairing an order
	/// @param acceptor: a weak_ptr to the acceptor of order
	/// @param order: a weak_ptr to order
//	virtual void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) = 0;

	/// @brief End repair an order
	/// @param acceptor: a weak_ptr to the acceptor
	/// @param order: a weak_ptr to order
	/// @param transaction: the price of the order
//	virtual void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) = 0;

	/// @brief Pay for the order
	/// @param committer: a weak_ptr to customer who commit the order
	/// @param order: a weak_ptr to order
//	virtual void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) = 0;

	/// @brief Get an order by it's id
	/// @return a weak_ptr to the order
//	virtual std::weak_ptr<Order> getOrder(unsigned long id) = 0;

protected:
	/// @brief Load orders for account
//	virtual void loadAllOrderForAccount(std::weak_ptr<Account> account) = 0;
};


#endif //HARSERVER_ORDERMANAGERSUBJECT_H
