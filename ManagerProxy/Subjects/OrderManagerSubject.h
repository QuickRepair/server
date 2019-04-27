//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ORDERMANAGERSUBJECT_H
#define HARSERVER_ORDERMANAGERSUBJECT_H

#include <memory>
#include <list>

class CustomerAccount;
class MerchantAccount;
class Order;
class Account;
class ContactInformation;
class AcceptableOrderPriceRange;

/// @brief The interface of OrderManager
/// @note: use subclass OrderManagerProxy instead of OrderManager
class OrderManagerSubject {
public:
	virtual ~OrderManagerSubject() = 0;

	virtual std::weak_ptr<Order> publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
					  std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) = 0;
	virtual void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) = 0;
	virtual void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) = 0;
	virtual void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) = 0;
	virtual void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) = 0;
	virtual void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) = 0;

	virtual void loadAllOrderForAccount(std::weak_ptr<Account> account) = 0;

	virtual std::weak_ptr<Order> getOrder(unsigned long id) = 0;
};


#endif //HARSERVER_ORDERMANAGERSUBJECT_H
