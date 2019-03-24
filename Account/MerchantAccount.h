#ifndef MERCHANTACCOUNT_H
#define MERCHANTACCOUNT_H

#include "Account.h"
#include <memory>

class ContactInformation;
class MerchantServiceType;

class MerchantAccount : public Account, public std::enable_shared_from_this<MerchantAccount>
{
	friend class MerchantFactory;

public:
    MerchantAccount(unsigned long id, std::string account, std::string password);

    void acceptOrder(std::weak_ptr<Order> order);
    void startRepair(std::weak_ptr<Order> order);
    void endRepair(std::weak_ptr<Order> order, double transaction);

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrders() const override;

	std::weak_ptr<MerchantServiceType> supportedServiceType();
	void updateSupportedService(std::list<std::string> appliancType, int maxDistance);

protected:
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrders(std::list<std::weak_ptr<Order>> orders) override;
	void loadServiceType(std::shared_ptr<MerchantServiceType> service);

private:
	std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::shared_ptr<MerchantServiceType> m_serviceType;
    std::list<std::weak_ptr<Order>> m_orders;
};

#endif // MERCHANTACCOUNT_H
