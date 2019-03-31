#ifndef HAR_DATABASE_H
#define HAR_DATABASE_H

#include <mariadb/mysql.h>
#include <tuple>
#include <chrono>
#include <memory>
#include <vector>
#include <sstream>
#include "../../Errors/AccountAlreadyExistError.h"
#include "QueryResult.h"
#include "../../Errors/DatabaseInternalError.h"
#include "../../Account/CustomerAccount.h"

class ContactInformation;
class OrderStateParameters;
class OrderStateAbstractFactory;

class DatabaseConnection {
public:
	void connect(std::string databaseIp, std::string databaseName, std::string databaseUser,
				 std::string databasePassword, unsigned databasePort);

	static DatabaseConnection &getInstance();

	/* unsigned long for order id
	 */
	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail);

	/* 1 unsigned long for order id
	 * 2 unsigned long for committer id
	 * 3 unsigned long for receiver id
	 * 4 string for appliance type
	 * 5 string for detail
	 * 6 unsigned long for current state id
	 */
	std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> queryOrderByAccountId(unsigned long id);

	/* in std::tuple<std::string, bool>
	 * 0 std::shared_ptr<OrderStateAbstractFactory> for state factory
	 * 1 OrderStateParameters for detail state information
	 */
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId);
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState);

	/* check password, if password is right,
	 * return id
	 */
	unsigned long checkMerchantPasswordAndGetId(std::string account, std::string password);
	unsigned long checkCustomerPasswordAndGetId(std::string account, std::string password);
	std::vector<std::tuple<>> queryContactInfoByUserId(unsigned long userId);

	/* create new account by giving account and password
	 */
	void createMerchantAccount(std::string account, std::string password);
	void createCustomerAccount(std::string account, std::string password);

	/* update account password
	 */
	void updateMerchantAccountPassword(std::string account, std::string password);
	void updateCustomerAccountPassword(std::string account, std::string password);

	/* Query merchant service type by merchant id
	 * return type:
	 * 0 std::list<std::string>> for supported types
	 * 1 int for max repair distance
	 */
	std::tuple<std::list<std::string>, int> queryMerchantServiceType(unsigned long id);

	/* Load account by id
	 * return type:
	 * 0 std::string for account
	 * 1 std::string for password
	 */
	std::tuple<std::string, std::string> loadMerchant(unsigned long id);
	std::tuple<std::string, std::string> loadCustomer(unsigned long id);

private:
	DatabaseConnection();
	~DatabaseConnection();

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
