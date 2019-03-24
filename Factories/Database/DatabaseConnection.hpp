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
	void conenct(std::string databaseIp, std::string databaseName, std::string databaseUser, std::string databasePassword, unsigned databasePort);

	static DatabaseConnection &getInstance();

	/* 1 std::string for detail
	 * 2 unsigned long for Order id
	 * 3 unsigned long for Order state id
	 */
	std::tuple<ContactInformation, std::string, unsigned long, unsigned long> queryOrderById(unsigned long orderId);

	/* in std::tuple<std::string, bool>
	 * 0 std::shared_ptr<OrderStateAbstractFactory> for state factory
	 * 1 OrderStateParameters for detail state information
	 */
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId);
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState);

	/* in std::tuple<unsigned long, std::string, std::string, std::string>
	 * 0 unsigned long for id
	 * 1 std::string for account
	 * 2 std::string for password
	 * 3 std::string for userName
	 */
	std::tuple<unsigned long, std::string, std::string, std::string> checkPasswordAndGetUserInfo(std::string account, std::string password);
	std::vector<std::tuple<>> queryContactInfoByUserId(unsigned long userId);

	/*
	 */
	template<typename AccountType>
	void createUserAndGenerageUserId(std::string account, std::string password)
	{
		// does the user exist
		std::string query =  "SELECT * FROM user WHERE account='" + account + "'";
		if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
			throw DatabaseInternalError("Create user, " + std::string(mysql_error(m_mysqlConnection)));
		QueryResult findResult = mysql_store_result(m_mysqlConnection);
		auto isExist = findResult.fetch_a_row();
		if(!isExist.empty())
			throw AccountAlreadyExistError("Faild to create, " + account + " already exist");

		// insert into database
		std::string type = (typeid(AccountType) == typeid(CustomerAccount)) ? "customer" : "merchant";
		query = "INSERT INTO user (account, password, user_type) VALUES ('" + account + "', '" + password + "', '" + type + "')";
		if(mysql_real_query(m_mysqlConnection, query.data(), query.length()))
			throw DatabaseInternalError("Create user, " + std::string(mysql_error(m_mysqlConnection)));
	}

	/*
	 */
	void updateUserPassword(std::string account, std::string password);

private:
	DatabaseConnection();
	~DatabaseConnection();

	unsigned long toUnsignedLong(std::string str);
	double toDouble(std::string str);
	std::chrono::system_clock::time_point toTimePoint(std::string str);
	std::shared_ptr<OrderStateAbstractFactory> findFactory(std::string orderType);

	static MYSQL *m_mysqlConnection;
};

#endif //HAR_DATABASE_H
