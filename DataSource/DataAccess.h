//
// Created by gaojian on 19-4-13.
//

#ifndef HARSERVER_DATAACCESS_H
#define HARSERVER_DATAACCESS_H

#include <memory>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <any>

class AcceptableOrderPriceRange;
class OrderEvaluate;

/// @brief The super class of all data sources, this is a singleton template,
/// no matter how many subclass is derived, only ONE instance will exist among them
/// @note: DataSource class does not define any operations but @getInstance(), define a subclass before using it
class DataAccess {
public:
	virtual ~DataAccess() = 0;

	virtual std::string getPasswordFor(unsigned long id) = 0;
	virtual void setPasswordFor(unsigned long id, const std::string &newPassword) = 0;
	virtual std::string getAccountFor(unsigned long id) = 0;
	virtual void setAccountFor(unsigned long id, const std::string &newAccount) = 0;

	virtual std::string getOrderApplianceTypeFor(unsigned long id) = 0;
	virtual std::string getOrderDetailFor(unsigned long id) = 0;

	virtual std::chrono::system_clock::time_point getOrderCreateDate(unsigned long id) = 0;
	virtual std::chrono::system_clock::time_point getOrderRejectDate(unsigned long id) = 0;
	virtual std::chrono::system_clock::time_point getOrderReceiveDate(unsigned long id) = 0;
	virtual std::chrono::system_clock::time_point getOrderStartRepairDate(unsigned long id) = 0;
	virtual std::chrono::system_clock::time_point getOrderEndRepairDate(unsigned long id) = 0;
	virtual std::chrono::system_clock::time_point getOrderFinishDate(unsigned long id) = 0;

	virtual AcceptableOrderPriceRange getAcceptablePriceRange(unsigned long id) = 0;
	virtual double getOrderTransaction(unsigned long id) = 0;
	virtual void setOrderEvaluate(OrderEvaluate &evaluate) = 0;
	virtual OrderEvaluate getOrderEvaluate(unsigned long id) = 0;

	virtual std::list<std::string> getMerchantSupportedApplianceTypes(unsigned long id) = 0;
	virtual void updateSupportedAppliances(unsigned long id, std::list<std::string> types) = 0;
	virtual double getMaxSupportDistance(unsigned long id) = 0;
	virtual void setMaxSupportDistance(unsigned long id, double distance) = 0;

	virtual std::list<std::string> getAllowedService() = 0;

	virtual unsigned long getOrderCommitter(unsigned long orderId) = 0;
	virtual unsigned long getOrderReceiver(unsigned long orderId) = 0;
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
//	virtual std::list<std::array<unsigned long, 3>> getOrdersByAccountId(unsigned long id) = 0;

	/// @brief Query order state according to the order id
	/// @param orderId: the order's id
	/// @return return state factory and state details,
	/// 		saved in a vector of tuple, tuple holds:
	///	@returns:
	/// 		a shared_prt to OrderStateAbstractFactory,
	/// 		the state details
	virtual std::list<std::any> getOrderStateByOrderId(unsigned long orderId) = 0;

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
//	virtual std::vector<std::tuple<std::string, std::string>> queryContactInfoByUserId(unsigned long userId) = 0;

	/// @brief create a new account
	/// @param account: account of the new account
	/// @param password: password of new account
	virtual void createOrUpdatePasswordForMerchantAccount(std::string account, std::string password) = 0;
	virtual void createOrUpdatePasswordForCustomerAccount(std::string account, std::string password) = 0;

	/// @brief update password of an account
	/// @param account: specific the account needs to be updated
	/// @param oldPassword: old password for account
	/// @param newPassword: new password for account
//	virtual void updateAccountPassword(std::string account, std::string newPassword) = 0;

	/// @brief Query merchant service type by merchant id
	/// @param id: the merchant id
	/// @return merchant service types, include support appliances and max repair distance,
	/// 		the tuple holds:
	///	@returns:
	/// 		a list of string for support appliances,
	/// 		max repair distance
	//virtual MerchantServiceType queryMerchantServiceType(unsigned long id) = 0;

	/// @brief Get account information about the id
	/// @param id: the account id
	/// @return the account and password of account, the tuple holds:
	/// @returns:
	///			account,
	///			password
//	virtual std::tuple<std::string, std::string> loadAccount(unsigned long id) = 0;

	/// @brief Update supported services into database
	/// @param id: the id for merchant
	/// @param types: supported types
	/// @param maxDistance: supported max repair distance
	virtual void updateMerchantServices(unsigned long id, std::list<std::string> types, double maxDistance) = 0;

	/// @brief Update order state
	/// @param id: the id for order
	virtual void orderReceived(unsigned long id) = 0;
	virtual void orderRepairing(unsigned long id) = 0;
	/// @param price: price for the order
	virtual void orderEndRepair(unsigned long id, double price) = 0;
	virtual void orderPayed(unsigned long id) = 0;
	virtual void orderFinished(unsigned long id) = 0;
	virtual void orderRejected(unsigned long id) = 0;
};

#endif //HARSERVER_DATAACCESS_H
