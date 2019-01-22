#ifndef HAR_DATABASE_H
#define HAR_DATABASE_H

#include <mariadb/mysql.h>
#include <tuple>
#include <chrono>
#include <memory>
#include <vector>

class AddressInformation;
class OrderStateParameters;
class OrderStateAbstractFactory;

class DatabaseConnection {
public:
	static DatabaseConnection &getInstance();

	//void test();
	/* 1 std::string for detail
	 * 2 unsigned long for order id
	 * 3 unsigned long for order state id
	 */
	std::tuple<AddressInformation, std::string, unsigned long, unsigned long> queryOrderById(unsigned long orderId);

	/* in std::tuple<std::string, bool>
	 * 0 std::shared_ptr<OrderStateAbstractFactory> for state factory
	 * 1 OrderStateParameters for detail state information
	 */
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId);
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
		queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState);

	/* in std::tuple<unsigned long, std::string, std::string, std::string>
	 * 0 unsigned long for user id
	 * 1 std::string for name
	 * 2 std::string for password
	 * 3 std::string for email
	 */
	std::tuple<unsigned long, std::string, std::string, std::string> checkPasswordAndGetUserInfo(std::string email, std::string password);
	std::vector<std::tuple<>> queryUserAddressByUserId(unsigned long userId);

private:
	DatabaseConnection();
	~DatabaseConnection();

	//throw exception
	void errorHappen(std::string err);

	unsigned long toUnsignedLong(std::string str);
	double toDouble(std::string str);
	std::chrono::system_clock::time_point toTimePoint(std::string str);
	std::shared_ptr<OrderStateAbstractFactory> findFactory(std::string orderType);

	static MYSQL *m_mysqlConnection;
};

#endif //HAR_DATABASE_H
