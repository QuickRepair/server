//
// Created by gaojian on 19-5-9.
//

#ifndef HARSERVER_POSTGRESCONNECTION_H
#define HARSERVER_POSTGRESCONNECTION_H

#include "DataSource/DataAccess.h"
#include <chrono>
#include <memory>
#include <pqxx/pqxx>

class PostgresConnection : public DataAccess {
public:
	PostgresConnection(std::string dbName, std::string userName, std::string password, std::string addr, unsigned port);
	~PostgresConnection() override = default;

	std::string getPasswordFor(AccountId id) override;
	void setPasswordFor(AccountId id, const std::string &newPassword) override;
	std::string getAccountFor(AccountId id) override;
	void setAccountFor(AccountId id, const std::string &newAccount) override;

	std::string getOrderApplianceTypeFor(OrderId id) override;
	std::string getOrderDetailFor(OrderId id) override;

	std::chrono::system_clock::time_point getOrderCreateDate(OrderId id) override;
	std::chrono::system_clock::time_point getOrderReceiveDate(OrderId id) override;
	std::chrono::system_clock::time_point getOrderStartRepairDate(OrderId id) override;
	std::chrono::system_clock::time_point getOrderEndRepairDate(OrderId id) override;
	std::chrono::system_clock::time_point getOrderFinishDate(OrderId id) override;
	std::chrono::system_clock::time_point getOrderRejectDate(OrderId id) override;

	AcceptableOrderPriceRange getAcceptablePriceRange(OrderId id) override;
	double getOrderTransaction(OrderId id) override;
	void setOrderEvaluate(OrderEvaluate evaluate) override;
	OrderEvaluate getOrderEvaluate(OrderId id) override;

	std::list<std::string> getMerchantSupportedApplianceTypes(AccountId id) override;
	void updateSupportedAppliances(AccountId id, std::list<std::string> types) override;
	double getMaxSupportDistance(AccountId id) override;
	void setMaxSupportDistance(AccountId id, double distance) override;
	std::list<OrderId> getOrderList(AccountId id) override;

	std::list<std::string> getAllowedService() override;

	std::list<AccountId> getAllMerchants() override;
	AccountId getOrderCommitter(OrderId orderId) override;
	AccountId getOrderReceiver(OrderId orderId) override;

	/// @override
	OrderId createOrder(AccountId committerId, AccountId acceptorId, std::string applianceType, std::string detail) override;
//	std::list<std::array<unsigned long, 3>> getOrdersByAccountId(unsigned long id) override;
	std::list<std::any> getOrderStateByOrderId(OrderId orderId) override;

	/// @override
	AccountId checkPasswordAndGetId(std::string account, std::string password) override;
//	std::vector<std::tuple<std::string, std::string>> queryContactInfoByUserId(unsigned long userId) override;

	/// @override
	void createOrUpdatePasswordForMerchantAccount(std::string account, std::string password) override;
	void createOrUpdatePasswordForCustomerAccount(std::string account, std::string password) override;

	/// @override
//	void updateAccountPassword(std::string account, std::string newPassword) override;

	/// @override
//	MerchantServiceType queryMerchantServiceType(unsigned long id) override;

	/// @override
//	std::tuple<std::string, std::string> loadAccount(unsigned long id) override;

	/// @override
	void updateMerchantServices(AccountId id, std::list<std::string> types, double maxDistance) override;

	/// @override
	void orderReceived(OrderId id) override;
	void orderRepairing(OrderId id) override;
	void orderEndRepair(OrderId id, double price) override;
	void orderPayed(OrderId id) override;
	void orderFinished(OrderId id) override;
	void orderRejected(OrderId id) override;

private:
	pqxx::connection m_connection;
};


#endif //HARSERVER_POSTGRESCONNECTION_H
