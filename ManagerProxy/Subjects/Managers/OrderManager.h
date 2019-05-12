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
class AccountManagerProxy;

class OrderManager : public OrderManagerSubject {
	friend class OrderManagerProxy;

public:
	static OrderManager &getInstance();

	/// @override
	std::weak_ptr<Order> publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
			std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) override;
	void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) override;
	void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) override;
	void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) override;


	/// @override
	std::weak_ptr<Order> getOrder(unsigned long id) override;

private:
	OrderManager();

	/// @override
	void loadAllOrderForAccount(std::weak_ptr<Account> account) override;

	/// @brief Check if the order is already loaded in account
	bool isTheOrderAlreadyLoaded(std::weak_ptr<Order> order, std::weak_ptr<Account> account);

	std::list<std::shared_ptr<Order>> m_orderList;
	std::shared_ptr<OrderFactory> m_factory;
	std::shared_ptr<AccountManagerProxy> m_accountManagerProxy;
};

#endif //HAR_ORDERMANAGESYSTEM_H
