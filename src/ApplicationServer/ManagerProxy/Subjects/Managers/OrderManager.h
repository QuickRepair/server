#ifndef HAR_ORDERMANAGESYSTEM_H
#define HAR_ORDERMANAGESYSTEM_H

#include <memory>
#include <unordered_map>
#include "ManagerProxy/Subjects/OrderManagerSubject.h"
#include "types.h"

class OrderFactory;
class Order;

class OrderManager : public OrderManagerSubject {
public:
	static OrderManager &getInstance();

	/// @override
	std::shared_ptr<Order> createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType, std::string detail) override;
	std::weak_ptr<Order> getOrder(OrderId id) override;
	std::unordered_map<OrderId, std::shared_ptr<Order>> loadAccountOrders(AccountId id) override;
	/*std::weak_ptr<Order> publishOrder(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<MerchantAccount> &acceptor,
			std::string &applianceType, ContactInformation &contactWay, std::string &detail, AcceptableOrderPriceRange &range) override;
	void orderAccepted(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderRejected(std::weak_ptr<MerchantAccount> &account, std::weak_ptr<Order> &order) override;
	void orderStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order) override;
	void orderEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction) override;
	void orderPayed(std::weak_ptr<CustomerAccount> &committer, std::weak_ptr<Order> &order) override;*/


	/// @override
//	std::weak_ptr<Order> getOrder(unsigned long id) override;

private:
	OrderManager();

	/// @override
//	void loadAllOrderForAccount(std::weak_ptr<Account> account) override;

	/// @brief Check if the order is already loaded in account
//	bool isTheOrderAlreadyLoaded(std::weak_ptr<Order> order, std::weak_ptr<Account> account);

//	std::list<std::shared_ptr<Order>> m_orderList;
	std::unordered_map<OrderId, std::shared_ptr<Order>> m_orders;
	std::unique_ptr<OrderFactory> m_factory;
//	std::shared_ptr<AccountManagerProxy> m_accountManagerProxy;
};

#endif //HAR_ORDERMANAGESYSTEM_H
