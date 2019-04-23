//
// Created by gaojian on 19-4-13.
//

#ifndef HARSERVER_DATASOURCE_HPP
#define HARSERVER_DATASOURCE_HPP

// define use which data source
#ifdef ENV_TEST
#define DataSourceFrom DataSource<SimulateDatabase>
#else
#define DataSourceFrom DataSource<DatabaseConnection>
#endif

#include <memory>
#include <list>
#include <tuple>
#include <vector>
#include <string>

class OrderStateAbstractFactory;
struct OrderStateParameters;

template<typename T>
class DataSource {
public:

	static DataSource &getInstance() {
		//TODO: not thread safe
		if(!m_dataSource)
		{
			// use the struct UniquePtrMaker to access protected/private constructor
			struct UniquePtrMaker : public T {
				static std::unique_ptr<T> make()
				{ return std::make_unique<UniquePtrMaker>(); }
			};
			m_dataSource = UniquePtrMaker::make();
		}
		return *m_dataSource.get();
	}

	/* parameters:
	 * 1 unsigned long the customer id
	 * 2 unsigned long the specific merchant id
	 * 3 string type of appliance
	 * 4 string detail description of order
	 *
	 * return value:
	 * 1 unsigned long for order id
	 */
	virtual unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) = 0;

	/* parameters:
	 * 1 unsigned long account id
	 *
	 * return value:
	 * 1 unsigned long for order id
	 * 2 unsigned long for committer id
	 * 3 unsigned long for receiver id
	 * 4 string for appliance type
	 * 5 string for detail
	 * 6 unsigned long for current state id
	 */
	virtual std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> queryOrderByAccountId(unsigned long id) = 0;

	/* parameters:
	 * 1 unsigned long order id
	 * 2 unsigned long the state id
	 *
	 * return value:
	 * 0 shared_ptr<OrderStateAbstractFactory> for state factory
	 * 1 OrderStateParameters for detail state information
	 */
	virtual std::list<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
	queryOrderStateByOrderId(unsigned long orderId) = 0;

	/* parameters:
	 * 1 string the account
	 * 2 string the password
	 *
	 * return value:
	 * unsigned long for id
	 */
	virtual unsigned long checkMerchantPasswordAndGetId(std::string account, std::string password) = 0;
	virtual unsigned long checkCustomerPasswordAndGetId(std::string account, std::string password) = 0;

	virtual std::vector<std::tuple<>> queryContactInfoByUserId(unsigned long userId) = 0;

	/* create account
	 * parameters:
	 * 1 string new account
	 * 2 string new password
	 */
	virtual void createMerchantAccount(std::string account, std::string password) = 0;
	virtual void createCustomerAccount(std::string account, std::string password) = 0;

	/* update account password
	 * parameters:
	 * 1 string update for which account
	 * 2 string new password
	 */
	virtual void updateMerchantAccountPassword(std::string account, std::string password) = 0;
	virtual void updateCustomerAccountPassword(std::string account, std::string password) = 0;

	/* Query merchant service type by merchant id
	 * parameters:
	 * 1 unsigned long merchant account id
	 *
	 * return value:
	 * 0 std::list<std::string>> for supported types
	 * 1 int for max repair distance
	 */
	virtual std::tuple<std::list<std::string>, int> queryMerchantServiceType(unsigned long id) = 0;

	/* Get account information by account id
	 * parameters:
	 * 1 unsigned long account id
	 *
	 * return value:
	 * 0 std::string for account
	 * 1 std::string for password
	 */
	virtual std::tuple<std::string, std::string> loadMerchant(unsigned long id) = 0;
	virtual std::tuple<std::string, std::string> loadCustomer(unsigned long id) = 0;

protected:
	DataSource() = default;
	virtual ~DataSource() = default;

private:
	static std::unique_ptr<T> m_dataSource;
};

template <typename T>
std::unique_ptr<T> DataSource<T>::m_dataSource;

#endif //HARSERVER_DATASOURCE_HPP
