
#ifndef HAR_ORDERFACTORY_H
#define HAR_ORDERFACTORY_H

#include <memory>
#include <string>
#include <list>

class Order;
class OrderState;
class Account;
class CustomerAccount;
class MerchantAccount;
class ContactInformation;
class AcceptableOrderPriceRange;
class OrderManagerProxy;
class AccountManagerProxy;

class OrderFactory {
public:
	OrderFactory();

	void getOrdersListForAccount(std::weak_ptr<Account> account);
	std::shared_ptr<Order> createOrder(std::weak_ptr<CustomerAccount> committer, std::weak_ptr<MerchantAccount> acceptor,
									   std::string applianceType, ContactInformation contactWay, std::string detail, AcceptableOrderPriceRange range);

private:
	std::shared_ptr<OrderState> getStates(std::shared_ptr<Order> &order);

	std::shared_ptr<OrderManagerProxy> orderManagerProxy;
	std::shared_ptr<AccountManagerProxy> accountManagerProxy;
};

#endif //HAR_ORDERFACTORY_H
