#ifndef HAR_ORDERMANAGESYSTEM_H
#define HAR_ORDERMANAGESYSTEM_H

#include <memory>
#include <list>
#include "ManagerProxy/Subjects/OrderManagerSubject.h"

class OrderFactory;
class CustomerAccount;
class MerchantAccount;
class Order;
class Account;
class ContactInformation;
class AcceptableOrderPriceRange;

class OrderManager : public OrderManagerSubject {
public:
	static OrderManager &getInstance();

	/// @override
	void publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
			std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) override;
	void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) override;
	void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) override;
	void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) override;

	/// @override
	void loadAllOrderForAccount(std::weak_ptr<Account> account) override;

	/// @override
	std::list<std::weak_ptr<Order>> getOrderList(std::weak_ptr<Account> &account) override;
	std::weak_ptr<Order> getOrder(unsigned long id) override;

private:
	OrderManager();

	std::list<std::shared_ptr<Order>> m_orderList;
	std::shared_ptr<OrderFactory> m_factory;
};

#endif //HAR_ORDERMANAGESYSTEM_H
