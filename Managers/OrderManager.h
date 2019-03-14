#ifndef HAR_ORDERMANAGESYSTEM_H
#define HAR_ORDERMANAGESYSTEM_H

#include <memory>
#include <list>

class OrderFactory;
class CustomerAccount;
class MerchantAccount;
class Order;
class Account;
class ContactInformation;
class AcceptableOrderPriceRange;

class OrderManager {
public:
	static OrderManager &getInstance();

	void publishOrder(std::weak_ptr<CustomerAccount> commiter, ContactInformation contact, std::string detail, AcceptableOrderPriceRange range);
	void requestAcceptOrder(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void requestStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void requestEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction);
	void requestPayOrder(std::weak_ptr<CustomerAccount> &commiter, std::weak_ptr<Order> &order);
	std::list<std::weak_ptr<Order>> requestQueryOrder(std::weak_ptr<Account> user);

private:
	OrderManager();

	std::list<std::shared_ptr<Order>> m_orders;
	std::shared_ptr<OrderFactory> m_factory;
};

#endif //HAR_ORDERMANAGESYSTEM_H
