#ifndef MERCHANTACCOUNT_H
#define MERCHANTACCOUNT_H

#include "CustomerAccount.h"
#include <memory>
#include <unordered_map>
/// unique_ptr error !!!
#include "MerchantService.h"
/// unique_ptr error !!!

//class MerchantService;

/// @brief a subclass of Account, represent a merchant who is using the system
class MerchantAccount : public CustomerAccount
{
public:
    explicit MerchantAccount(unsigned long id);

    /// @brief MerchantAccount specific operation, register an not accepted order belong to the merchant
    /// @param order: a weak_ptr to the order
	void orderWaitToBeAccept(std::weak_ptr<Order> order);

	/// @brief MerchantAccount specific operation, accept an order which was registered by @orderWaitToBeAccept()
	/// @param order: a weak_ptr to the order
//    void acceptOrder(std::weak_ptr<Order> order);

    /// @brief MerchantAccount specific operation, start repair an order belong to the merchant
    /// @param order: a weak_ptr to the order
//    void startRepair(std::weak_ptr<Order> order);

    /// @brief MerchantAccount specific operation, end repair an order belong to the merchant
    /// @param order: a weak_ptr to the order
    /// @param transaction: the price of the order
//    void endRepair(std::weak_ptr<Order> order, double transaction);

    /// @brief MerchantAccount specific operation, reject an order which was registered by @orderWaitToBeAccept()
//	void rejectOrder(std::weak_ptr<Order> order);

	/// @brief Test if the order belong to the merchant submitted/processed/unreceived orders
	/// @param order: a weak_ptr to the order
	/// @return true if the order is in the list, false on the contrary
//	bool isMySubmittedOrder(std::weak_ptr<Order> order) const;
//	bool isMyProcessedOrder(std::weak_ptr<Order> order) const;
//	bool isMyUnreceivedOrder(std::weak_ptr<Order> order) const;

	/// @brief Get all unreceived orders
	/// @return a list of weak_ptr to unreceived orders
//	std::list<std::weak_ptr<Order>> myUnreceivedOrderList() const;

    /// @override
//    bool isMyOrder(std::weak_ptr<Order> order) const override;
//	std::list<std::weak_ptr<Order>> myOrdersList() const override;

	/// @brief Get merchant supported service type
	/// @return a ptr to service type
	MerchantService *getService();

	/// @brief Update the merchant service type
	/// @param appliancType: a list of supported appliance type
	/// @param maxDistance: the merchant max service distance
//	void updateSupportedService(std::list<std::string> appliancType, double maxDistance);

protected:
    /// @override
//	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
//	void loadOrder(std::shared_ptr<Order> order) override;

	/// @brief Load merchant service type
	/// @param service: a shared_ptr to service type
//	void loadServiceType(std::unique_ptr<MerchantService> &&service);

private:
    std::unique_ptr<MerchantService> m_serviceType;
//    std::unordered_map<unsigned long, std::shared_ptr<Order>> m_processedOrders;
//    std::unordered_map<unsigned long, std::shared_ptr<Order>> m_unreceivedOrders;
};

#endif // MERCHANTACCOUNT_H
