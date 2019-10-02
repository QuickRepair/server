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

	std::string getPasswordFor(unsigned long id) override;
	void setPasswordFor(unsigned long id, const std::string &newPassword) override;
	std::string getAccountFor(unsigned long id) override;
	void setAccountFor(unsigned long id, const std::string &newAccount) override;

	std::string getOrderApplianceTypeFor(unsigned long id) override;
	std::string getOrderDetailFor(unsigned long id) override;

	std::chrono::system_clock::time_point getOrderCreateDate(unsigned long id) override;
	std::chrono::system_clock::time_point getOrderReceiveDate(unsigned long id) override;
	std::chrono::system_clock::time_point getOrderStartRepairDate(unsigned long id) override;
	std::chrono::system_clock::time_point getOrderEndRepairDate(unsigned long id) override;
	std::chrono::system_clock::time_point getOrderFinishDate(unsigned long id) override;
	std::chrono::system_clock::time_point getOrderRejectDate(unsigned long id) override;

	AcceptableOrderPriceRange getAcceptablePriceRange(unsigned long id) override;
	double getOrderTransaction(unsigned long id) override;
	void setOrderEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate getOrderEvaluate(unsigned long id) override;

	std::list<std::string> getMerchantSupportedApplianceTypes(unsigned long id) override;
	void updateSupportedAppliances(unsigned long id, std::list<std::string> types) override;
	double getMaxSupportDistance(unsigned long id) override;
	void setMaxSupportDistance(unsigned long id, double distance) override;

	std::list<std::string> getAllowedService() override;

	unsigned long getOrderCommitter(unsigned long orderId) override;
	unsigned long getOrderReceiver(unsigned long orderId) override;

	/// @override
	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) override;
//	std::list<std::array<unsigned long, 3>> getOrdersByAccountId(unsigned long id) override;
	std::list<std::any> getOrderStateByOrderId(unsigned long orderId) override;

	/// @override
	unsigned long checkPasswordAndGetId(std::string account, std::string password) override;
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
	void updateMerchantServices(unsigned long id, std::list<std::string> types, double maxDistance) override;

	/// @override
	void orderReceived(unsigned long id) override;
	void orderRepairing(unsigned long id) override;
	void orderEndRepair(unsigned long id, double price) override;
	void orderPayed(unsigned long id) override;
	void orderFinished(unsigned long id) override;
	void orderRejected(unsigned long id) override;

private:
	pqxx::connection m_connection;
	pqxx::work work;
};


#endif //HARSERVER_POSTGRESCONNECTION_H
