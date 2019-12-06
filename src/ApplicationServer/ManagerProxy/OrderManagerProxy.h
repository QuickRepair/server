//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ORDERMANAGERPROXY_H
#define HARSERVER_ORDERMANAGERPROXY_H

#include "ManagerProxy/Subjects/OrderManagerSubject.h"

class AccountManager;

class OrderManagerProxy : public OrderManagerSubject {
//	friend class AccountManager;
public:
	std::shared_ptr<Order> createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType, std::string detail) override;
	std::weak_ptr<Order> getOrder(OrderId id) override;
	std::unordered_map<OrderId, std::shared_ptr<Order>> loadAccountOrders(AccountId id) override;
	/// @override
	/*std::weak_ptr<Order> publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
					  std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) override;
	void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) override;
	void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) override;
	void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) override;*/

	/// @override
//	std::weak_ptr<Order> getOrder(unsigned long id) override;

protected:
	/// @override
//	void loadAllOrderForAccount(std::weak_ptr<Account> account) override;
};


#endif //HARSERVER_ORDERMANAGERPROXY_H
