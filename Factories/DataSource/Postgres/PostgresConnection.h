//
// Created by gaojian on 19-5-9.
//

#ifndef HARSERVER_POSTGRESCONNECTION_H
#define HARSERVER_POSTGRESCONNECTION_H

#include "Factories/DataSource/DataSource.hpp"
#include <chrono>
#include <memory>
#include <pqxx/pqxx>

class PostgresConnection : public DataSource<PostgresConnection> {

	friend class DataSource<PostgresConnection>;
	friend std::unique_ptr<PostgresConnection>::deleter_type;

public:
	/// @override
	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) override;

	/// @override
	std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string>> queryOrderByAccountId(unsigned long id) override;

	/// @override
	std::vector<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
	queryOrderStateByOrderId(unsigned long orderId) override;

	/// @override
	unsigned long checkPasswordAndGetId(std::string account, std::string password) override;
	std::vector<std::tuple<std::string, std::string>> queryContactInfoByUserId(unsigned long userId) override;

	/// @override
	void createMerchantAccount(std::string account, std::string password) override;
	void createCustomerAccount(std::string account, std::string password) override;

	/// @override
	void updateAccountPassword(std::string account, std::string newPassword) override;

	/// @override
	std::tuple<std::list<std::string>, double> queryMerchantServiceType(unsigned long id) override;

	/// @override
	std::tuple<std::string, std::string> loadAccount(unsigned long id) override;

protected:
	PostgresConnection();
	~PostgresConnection() override = default;

private:
	std::unique_ptr<pqxx::connection> m_connection;
};


#endif //HARSERVER_POSTGRESCONNECTION_H
