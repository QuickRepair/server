#if 0
#ifndef HAR_DATABASE_H
#define HAR_DATABASE_H

#include <mysql/mysql.h>
#include <tuple>
#include <chrono>
#include <memory>
#include <vector>
#include <sstream>
#include "Errors/AccountAlreadyExistError.hpp"
#include "Errors/DatabaseInternalError.hpp"
#include "QueryResult.h"
#include "Factories/DataSource/DataSource.hpp"
#include "Account/CustomerAccount.h"

class ContactInformation;
struct OrderStateParameters;
class OrderStateAbstractFactory;

class MariaDBConnection : public DataSource<MariaDBConnection> {

	friend class DataSource<MariaDBConnection>;
	friend std::unique_ptr<MariaDBConnection>::deleter_type;

public:

	/// @override
	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) override;

	/// @override
	std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> queryOrderByAccountId(unsigned long id) override;

	/// @override
	std::vector<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
	queryOrderStateByOrderId(unsigned long orderId) override;

	/// @override
	unsigned long checkMerchantPasswordAndGetId(std::string account, std::string password) override;
	unsigned long checkCustomerPasswordAndGetId(std::string account, std::string password) override;
	std::vector<std::tuple<>> queryContactInfoByUserId(unsigned long userId) override;

	/// @override
	void createMerchantAccount(std::string account, std::string password) override;
	void createCustomerAccount(std::string account, std::string password) override;

	/// @override
	void updateMerchantAccountPassword(std::string account, std::string password) override;
	void updateCustomerAccountPassword(std::string account, std::string password) override;

	/// @override
	std::tuple<std::list<std::string>, int> queryMerchantServiceType(unsigned long id) override;

	/// @override
	std::tuple<std::string, std::string> loadMerchant(unsigned long id) override;
	std::tuple<std::string, std::string> loadCustomer(unsigned long id) override;

protected:
	MariaDBConnection();
	~MariaDBConnection() override;

private:
	/// @brief Cast functions
	unsigned long toUnsignedLong(std::string str);
	int toInt(std::string str);
	double toDouble(std::string str);
	std::chrono::system_clock::time_point toTimePoint(std::string str);

	/// @brief Find factory fit order state
	/// @param orderState: the order state
	/// @return a shared_ptr to OrderStateAbstractFactory fit the given state
	std::shared_ptr<OrderStateAbstractFactory> findFactory(std::string orderType);

	/// @brief create new account
	/// @param account: new account
	/// @param password: new password
	/// @param accountType: "merchant" or "customer"
	void createAccount(std::string account, std::string password, std::string accountType);

	/// @brief update account information
	/// @param account: account
	/// @param password: new password
	/// @param accountType: "merchant" or "customer"
	void updateAccount(std::string account, std::string password, std::string accountType);

	/// @brief check password for account
	/// @param account: account
	/// @param password: password for account
	/// @param accountType: "merchant" or "customer"
	/// @return the id of the account
	unsigned long checkPasswordAndGetId(std::string account, std::string password, std::string accountType);

	static MYSQL *m_mysqlConnection;
};

#endif //HAR_DATABASE_H
#endif