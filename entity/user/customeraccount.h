#ifndef CUSTOMERACCOUNT_H
#define CUSTOMERACCOUNT_H

#include "user.h"
#include <list>
#include <memory>

class AddressInformation;
class Order;
class OrderFactory;
class OrderPriceRange;
class Evaluate;
template<typename UserType>
class UserFactory;

class CustomerAccount : public User
{
	friend class UserFactory<CustomerAccount>;
public:
    CustomerAccount(unsigned long id, std::string name, std::string password, std::string email);

	void submitOrder(std::weak_ptr<Order> order);
    void cancelOrder(std::weak_ptr<Order> order);
    void evaluateTheOrder(std::weak_ptr<Order> order, Evaluate evaluate);
    void payTheOrder(std::weak_ptr<Order> order);

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::shared_ptr<Order>> myOrders() const override;

protected:
	void loadAddress(std::list<std::shared_ptr<AddressInformation>> address) override;
	void loadOrders(std::list<std::shared_ptr<Order>> orders) override;

private:
    std::list<std::shared_ptr<AddressInformation>> m_address;
    std::list<std::shared_ptr<Order>> m_orders;
    std::shared_ptr<OrderFactory> m_orderFactory;
};

#endif // CUSTOMERACCOUNT_H
