#ifndef CUSTOMERACCOUNT_H
#define CUSTOMERACCOUNT_H

#include "Account.h"
#include <list>
#include <memory>

class ContactInformation;
class Order;
class OrderFactory;
class AcceptableOrderPriceRange;
class OrderEvaluate;
template<typename UserType>
class UserFactory;

class CustomerAccount : public Account
{
	friend class UserFactory<CustomerAccount>;
public:
    CustomerAccount(unsigned long id, std::string password, std::string userName);

	void iAmPublishAnOrder(std::weak_ptr<Order> order);
    void cancelOrder(std::weak_ptr<Order> order);
    void evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate);
    void payTheOrder(std::weak_ptr<Order> order);

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrders() const override;

protected:
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrders(std::list<std::weak_ptr<Order>> orders) override;

private:
    std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::list<std::weak_ptr<Order>> m_orders;
};

#endif // CUSTOMERACCOUNT_H
