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

	void publishOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
			std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range);
	void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order);
	void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order);
	void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction);
	void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order);
	std::list<std::weak_ptr<Order>> getOrderList(std::weak_ptr<Account> account);
	std::weak_ptr<Order> getOrder(unsigned long id);

private:
	OrderManager();

	std::list<std::shared_ptr<Order>> m_orderList;
	std::shared_ptr<OrderFactory> m_factory;
};

#endif //HAR_ORDERMANAGESYSTEM_H
