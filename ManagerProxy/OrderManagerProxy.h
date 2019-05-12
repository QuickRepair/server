//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ORDERMANAGERPROXY_H
#define HARSERVER_ORDERMANAGERPROXY_H

#include "ManagerProxy/Subjects/OrderManagerSubject.h"

class AccountManager;

class OrderManagerProxy : public OrderManagerSubject {
	friend class AccountManager;
public:
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

protected:
	/// @override
	void loadAllOrderForAccount(std::weak_ptr<Account> account) override;
};


#endif //HARSERVER_ORDERMANAGERPROXY_H
