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
	unsigned long createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail) override;

	std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> queryOrderByAccountId(unsigned long id) override;

	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
	queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId) override;
	std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
	queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState) override;

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
	SimulateDatabase() = default;
	~SimulateDatabase() override = default;

private:
	/* simulate account table
	 * id	account	password	type	max_service_distance
	 */
	std::list<std::tuple<unsigned, std::string, std::string, std::string, unsigned>> m_account;

	/* simulate merchant_service_type
	 * merchant_id	type
	 */
	std::list<std::tuple<unsigned, std::string>> m_serviceType;

	/* simulate orders
	 * id	committer	acceptor	appliance_type	detail	current_state
	 */
	std::list<std::tuple<unsigned, unsigned, unsigned, std::string, std::string, unsigned>> m_orders;

	/* simulate order_states
	 * order_id	id	last_state	date	transaction	state_name
	 */
	std::list<std::tuple<unsigned, unsigned, unsigned, std::chrono::system_clock::time_point, double, std::string>> m_orderStates;
};


#endif //HARSERVER_SIMULATEDATABASE_H