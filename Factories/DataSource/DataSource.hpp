//
// Created by gaojian on 19-4-13.
//

#ifndef HARSERVER_DATASOURCE_HPP
#define HARSERVER_DATASOURCE_HPP

/// @brief The DATA_SOURCE_FROM marco define which data source is used
#ifdef ENV_TEST
	#define DATA_SOURCE_FROM DataSource<SimulateDatabase>
#else
	#define DATA_SOURCE_FROM DataSource<PostgresConnection>
#endif

#include <memory>
#include <list>
#include <tuple>
#include <vector>
#include <string>

class OrderStateAbstractFactory;
struct OrderStateParameters;

/// @brief The super class of all data sources, this is a singleton template,
/// no matter how many subclass is derived, only ONE instance will exist among them
/// @note: DataSource class does not define any operations but @getInstance(), define a subclass befor using it
template<typename T>
class DataSource {
public:

	/// @brief Get instance of singleton
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

	/// @brief Create a new order and return generated id for the order
	/// @param committerId: the customer id
	/// @param acceptorId: the merchant id
	/// @param applianceType: type of appliance
	/// @param detail: detail description of order
	/// @return the new order id
	virtual unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) = 0;

	/// @brief Query all order that the account hold
	/// @param id: the id of an account
	/// @return return all orders belong to the account,
	/// 		saved in a vector of tuple, tuple holds:
	/// @returns:
	/// 		order id,
	/// 		committer id,
	/// 		receiver id,
	/// 		appliance type,
	/// 		detail
	virtual std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string>>
	queryOrderByAccountId(unsigned long id) = 0;

	/// @brief Query order state according to the order id
	/// @param orderId: the order's id
	/// @return return state factory and state details,
	/// 		saved in a vector of tuple, tuple holds:
	///	@returns:
	/// 		a shared_prt to OrderStateAbstractFactory,
	/// 		the state details
	virtual std::vector<std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>>
	queryOrderStateByOrderId(unsigned long orderId) = 0;

	/// @brief check password of account and get account id
	/// @param account: the account to be verified, @note: not account id
	/// @param password: the input password of account
	/// @return id of account
	virtual unsigned long checkPasswordAndGetId(std::string account, std::string password) = 0;

	/// @brief Get account contact information
	/// @param userId: the account id
	/// @return return the contact information in strings
	/// @returns:
	///			a string of address
	///			a string of telephone
	virtual std::vector<std::tuple<std::string, std::string>> queryContactInfoByUserId(unsigned long userId) = 0;

	/// @brief create a new account
	/// @param account: account of the new account
	/// @param password: password of new account
	virtual void createMerchantAccount(std::string account, std::string password) = 0;
	virtual void createCustomerAccount(std::string account, std::string password) = 0;

	/// @brief update password of an account
	/// @param account: specific the account needs to be updated
	/// @param oldPassword: old password for account
	/// @param newPassword: new password for account
	virtual void updateAccountPassword(std::string account, std::string newPassword) = 0;

	/// @brief Query merchant service type by merchant id
	/// @param id: the merchant id
	/// @return merchant service types, include support appliances and max repair distance,
	/// 		the tuple holds:
	///	@returns:
	/// 		a list of string for support appliances,
	/// 		max repair distance
	virtual std::tuple<std::list<std::string>, double> queryMerchantServiceType(unsigned long id) = 0;

	/// @brief Get account information about the id
	/// @param id: the account id
	/// @return the account and password of account, the tuple holds:
	/// @returns:
	///			account,
	///			password
	virtual std::tuple<std::string, std::string> loadAccount(unsigned long id) = 0;

	/// @brief Update supported services into database
	/// @param id: the id for merchant
	/// @param types: supported types
	/// @param maxDistance: supported max repair distance
	virtual void updateSupportedServices(unsigned long id, std::list<std::string> types, double maxDistance) = 0;

protected:
	DataSource() = default;
	virtual ~DataSource() = default;

private:
	static std::unique_ptr<T> m_dataSource;
};

template <typename T>
std::unique_ptr<T> DataSource<T>::m_dataSource;

#endif //HARSERVER_DATASOURCE_HPP
