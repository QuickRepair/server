#ifndef MERCHANTACCOUNT_H
#define MERCHANTACCOUNT_H

#include "user.h"
#include <memory>

class AddressInformation;
class MerchantServerType;
template<typename UserType>
class UserFactory;

class MerchantAccount : public User, public std::enable_shared_from_this<MerchantAccount>
{
	friend class UserFactory<MerchantAccount>;

public:
    MerchantAccount(unsigned long id, std::string name, std::string password, std::string email);

    void acceptOrder(std::weak_ptr<Order> order);
    void startRepair(std::weak_ptr<Order> order);
    void endRepair(std::weak_ptr<Order> order, double transaction);

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::shared_ptr<Order>> myOrders() const override;

protected:
	void loadAddress(std::list<std::shared_ptr<AddressInformation>> address) override;
	void loadOrders(std::list<std::shared_ptr<Order>> orders) override;

private:
    std::shared_ptr<AddressInformation> m_address;
    std::shared_ptr<MerchantServerType> m_serverType;
    std::list<std::shared_ptr<Order>> m_orders;
};

#endif // MERCHANTACCOUNT_H
