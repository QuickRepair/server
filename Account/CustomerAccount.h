#ifndef CUSTOMERACCOUNT_H
#define CUSTOMERACCOUNT_H

#include "Account.h"
#include <list>
#include <memory>

class ContactInformation;
class Order;
class AcceptableOrderPriceRange;
class OrderEvaluate;

/// @brief a subclass of Account, represent a customer who is using the system
class CustomerAccount : public Account
{
	friend class CustomerFactory;
	friend class OrderFactory;
	friend class OrderManager;

public:
    CustomerAccount(unsigned long id, std::string account, std::string password);

    /// @brief CustomerAccount specific operation, publish a new order to system
    /// @param order: a weak_ptr to the new order
	void iAmPublishAnOrder(std::weak_ptr<Order> order);

	/// @brief CustomerAccount specific operation, cancel a order belong to the customer
	/// @param order: a weak_ptr to the order
	// TODO: add some cancelable limit here
    void cancelOrder(std::weak_ptr<Order> order);

    /// @brief CustomerAccount specific operation, evaluate the order after finish repair
    /// @param order: a weak_ptr to the order
    /// @param evaluate: the evaluate add to order
    void evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate);

    /// @brief CustomerAccount specific operation, pay for the order after finish repair
    /// @param order: a weak_ptr to the order
    void payTheOrder(std::weak_ptr<Order> order);

    ///@override
    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrdersList() const override;

protected:
    ///@override
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrder(std::shared_ptr<Order> order) override;

    std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::list<std::shared_ptr<Order>> m_publishedOrders;
};

#endif // CUSTOMERACCOUNT_H
