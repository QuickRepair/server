//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ORDERMANAGERPROXY_H
#define HARSERVER_ORDERMANAGERPROXY_H

#include "ManagerProxy/Subjects/OrderManagerSubject.h"

class OrderManagerProxy : public OrderManagerSubject {
public:
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
};


#endif //HARSERVER_ORDERMANAGERPROXY_H
