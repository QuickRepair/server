#ifndef HAR_DATABASE_H
#define HAR_DATABASE_H

#include <mysql/mysql.h>
#include <tuple>
#include <chrono>
#include <memory>
#include <vector>
#include <sstream>
#include "Errors/AccountAlreadyExistError.h"
#include "Errors/DatabaseInternalError.h"
#include "QueryResult.h"
#include "Factories/DataSource/DataSource.hpp"
#include "Account/CustomerAccount.h"

class ContactInformation;
struct OrderStateParameters;
class OrderStateAbstractFactory;

class DatabaseConnection : public DataSource<DatabaseConnection> {

	friend class DataSource<DatabaseConnection>;
	friend std::unique_ptr<DatabaseConnection>::deleter_type;

public:

	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) override;

	std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> queryOrderByAccountId(unsigned long id) override;

	std::list<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
	queryOrderStateByOrderId(unsigned long orderId) override;

	unsigned long checkMerchantPasswordAndGetId(std::string account, std::string password) override;
	unsigned long checkCustomerPasswordAndGetId(std::string account, std::string password) override;
	std::vector<std::tuple<>> queryContactInfoByUserId(unsigned long userId) override;

	void createMerchantAccount(std::string account, std::string password) override;
	void createCustomerAccount(std::string account, std::string password) override;

	void updateMerchantAccountPassword(std::string account, std::string password) override;
	void updateCustomerAccountPassword(std::string account, std::string password) override;

	std::tuple<std::list<std::string>, int> queryMerchantServiceType(unsigned long id) override;

	std::tuple<std::string, std::string> loadMerchant(unsigned long id) override;
	std::tuple<std::string, std::string> loadCustomer(unsigned long id) override;

protected:
	DatabaseConnection();
	~DatabaseConnection() override;

private:
	unsigned long toUnsignedLong(std::string str);
	int toInt(std::string str);
	double toDouble(std::string str);
	std::chrono::system_clock::time_point toTimePoint(std::string str);
	std::shared_ptr<OrderStateAbstractFactory> findFactory(std::string orderType);
	void createAccount(std::string account, std::string password, std::string accountType);
	void updateAccount(std::string account, std::string password, std::string accountType);
	unsigned long checkPasswordAndGetId(std::string account, std::string password, std::string accountType);

	static MYSQL *m_mysqlConnection;
};

#endif //HAR_DATABASE_H
