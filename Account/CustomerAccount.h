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

class CustomerAccount : public Account
{
	friend class CustomerFactory;
	friend class OrderFactory;

public:
    CustomerAccount(unsigned long id, std::string account, std::string password);

	void iAmPublishAnOrder(std::weak_ptr<Order> order);
    void cancelOrder(std::weak_ptr<Order> order);
    void evaluateTheOrder(std::weak_ptr<Order> order, OrderEvaluate evaluate);
    void payTheOrder(std::weak_ptr<Order> order);

    bool isMyOrder(std::weak_ptr<Order> order) const override;
	std::list<std::weak_ptr<Order>> myOrdersList() const override;

protected:
	void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) override;
	void loadOrder(std::shared_ptr<Order> order) override;

private:
    std::list<std::shared_ptr<ContactInformation>> m_contactInfo;
    std::list<std::shared_ptr<Order>> m_orders;
};

#endif // CUSTOMERACCOUNT_H
