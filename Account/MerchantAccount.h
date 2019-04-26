#ifndef MERCHANTACCOUNT_H
#define MERCHANTACCOUNT_H

#include "Account.h"
#include <memory>

class ContactInformation;
class MerchantServiceType;

/// @brief a subclass of Account, represent a merchant who is using the system
class MerchantAccount : public Account, public std::enable_shared_from_this<MerchantAccount>
{
	friend class MerchantFactory;
	friend class OrderFactory;
	friend class OrderManager;

public:
    MerchantAccount(unsigned long id, std::string account, std::string password);

    /// @brief MerchantAccount specific operation, register an not accepted order belong to the merchant
    /// @param order: a weak_ptr to the order
	void orderWaitToBeAccept(std::shared_ptr<Order> order);

	/// @brief MerchantAccount specific operation, accept an order which was registered by @orderWaitToBeAccept()
	/// @param order: a weak_ptr to the order
    void acceptOrder(std::weak_ptr<Order> order);

    /// @brief MerchantAccount specific operation, start repair an order belong to the merchant
    /// @param order: a weak_ptr to the order
    void startRepair(std::weak_ptr<Order> order);

    /// @brief MerchantAccount specific operation, end repair an order belong to the merchant
    /// @param order: a weak_ptr to the order
    /// @param transaction: the price of the order
    void endRepair(std::weak_ptr<Order> order, double transaction);

    /// @brief MerchantAccount specific operation, reject an order which was registered by @orderWaitToBeAccept()
	void rejectOrder(std::weak_ptr<Order> order);

	/// @brief Test if the order belong to the merchant unreceived orders
	/// @param order: a weak_ptr to the order
	/// @return true if the order is in the list, false on the contrary
	bool isMyUnreceivedOrder(std::weak_ptr<Order> order);

	/// @brief Get all unreceived orders
	/// @return a list of weak_ptr to unreceived orders
	std::list<std::weak_ptr<Order>> myUnreceivedOrderList() const;

    /// @override
    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrdersList() const override;

	/// @brief Get merchant supported service type
	/// @return a weak_ptr to service type
	std::weak_ptr<MerchantServiceType> supportedServiceType();

	/// @brief Update the merchant service type
	/// @param appliancType: a list of supported appliance type
	/// @param maxDistance: the merchant max service distance
	void updateSupportedService(std::list<std::string> appliancType, int maxDistance);

protected:
    /// @override
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrder(std::shared_ptr<Order> order) override;

	/// @brief Load merchant service type
	/// @param service: a shared_ptr to service type
	void loadServiceType(std::shared_ptr<MerchantServiceType> service);

private:
	std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::shared_ptr<MerchantServiceType> m_serviceType;
    std::list<std::shared_ptr<Order>> m_receivedOrders;
    std::list<std::shared_ptr<Order>> m_unreceivedOrders;
};

#endif // MERCHANTACCOUNT_H
