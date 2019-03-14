#ifndef MERCHANTACCOUNT_H
#define MERCHANTACCOUNT_H

#include "Account.h"
#include <memory>

class ContactInformation;
class MerchantServiceType;
template<typename UserType>
class UserFactory;

class MerchantAccount : public Account, public std::enable_shared_from_this<MerchantAccount>
{
	friend class UserFactory<MerchantAccount>;

public:
    MerchantAccount(unsigned long id, std::string password, std::string userName);

    void acceptOrder(std::weak_ptr<Order> order);
    void startRepair(std::weak_ptr<Order> order);
    void endRepair(std::weak_ptr<Order> order, double transaction);
	std::weak_ptr<MerchantAccount> myConciseInfo();

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrders() const override;

protected:
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrders(std::list<std::weak_ptr<Order>> orders) override;

private:
	std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::shared_ptr<MerchantServiceType> m_serverType;
    std::list<std::weak_ptr<Order>> m_orders;
};

#endif // MERCHANTACCOUNT_H
