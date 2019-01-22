#ifndef HAR_ORDERMANAGESYSTEM_H
#define HAR_ORDERMANAGESYSTEM_H

#include <memory>
#include <list>

class OrderFactory;
class CustomerAccount;
class MerchantAccount;
class Order;
class User;
class AddressInformation;
class OrderPriceRange;

class OrderManageSystem {
public:
	static OrderManageSystem getInstance();

	std::string shunt_message(const std::string &data);

	void requestUploadOrder(std::weak_ptr<CustomerAccount> &commiter, AddressInformation address, std::string detail, OrderPriceRange range);
	void requestAcceptOrder(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void requestStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void requestEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction);
	void requestPayOrder(std::weak_ptr<CustomerAccount> &commiter, std::weak_ptr<Order> &order);
	std::list<std::shared_ptr<Order>> requestQueryOrder(std::weak_ptr<User> user);

private:
	OrderManageSystem();

	std::shared_ptr<OrderFactory> m_factory;
};

#endif //HAR_ORDERMANAGESYSTEM_H
