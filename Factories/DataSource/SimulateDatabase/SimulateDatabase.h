//
// Created by gaojian on 19-4-13.
//

#ifndef HARSERVER_SIMULATEDATABASE_H
#define HARSERVER_SIMULATEDATABASE_H

#include "Factories/DataSource/DataSource.hpp"
#include <chrono>

class SimulateDatabase : public DataSource<SimulateDatabase> {

	friend class DataSource<SimulateDatabase>;
	friend std::unique_ptr<SimulateDatabase>::deleter_type;

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
	SimulateDatabase();
	~SimulateDatabase() override = default;

private:
	/// @brief Find factory fit order state
	/// @param orderState: the order state
	/// @return a shared_ptr to OrderStateAbstractFactory fit the given state
	std::shared_ptr<OrderStateAbstractFactory> findFactory(std::string orderState);

	/* simulate account table
	 * id	account	password	type	max_service_distance
	 */
	std::list<std::tuple<unsigned long, std::string, std::string, std::string, unsigned long>> m_account;

	/* simulate merchant_service_type
	 * type	merchant_id
	 */
	std::list<std::tuple<std::list<std::string>, unsigned long>> m_serviceType;

	/* simulate orders
	 * id	committer	acceptor	appliance_type	detail	current_state
	 */
	std::list<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> m_orders;

	/* simulate order_states
	 * order_id	id	last_state	date	transaction	state_name
	 */
	std::list<std::tuple<unsigned long, int, unsigned long, std::chrono::system_clock::time_point, double, std::string>> m_orderStates;
};


#endif //HARSERVER_SIMULATEDATABASE_H
