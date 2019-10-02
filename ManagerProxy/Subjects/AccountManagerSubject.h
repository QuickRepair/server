//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ACCOUNTMANAGERSUBJECT_H
#define HARSERVER_ACCOUNTMANAGERSUBJECT_H

//#include <list>
//#include <memory>

//class AuthenticationCarrier;
class MerchantAccount;
class CustomerAccount;
//class Order;
//class MerchantServiceType;

#include "Account/Account.h"

/// @brief The interface of AccountManager
/// @note: use subclass AccountManagerProxy instead of AccountManager
class AccountManagerSubject {
public:
	virtual ~AccountManagerSubject() = 0;

	virtual Account *loadAccount(Account::Type type, unsigned long id) = 0;
	virtual void createOrUpdatePasswordForAccount(Account::Type type, std::string account, std::string password) = 0;
	virtual unsigned long getAccountIdIfPasswordRight(std::string account, std::string password) = 0;
	/// @brief Register an authentication carrier into manager
//	virtual void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier) = 0;

	/// @brief Get all merchant loaded
	/// @return a list of weak_prt to merchant account
	virtual std::list<MerchantAccount *> getMerchantList() = 0;

	/// @brief Get a merchant account
	/// @param id or account: the merchant account's id or account
	/// @return a weak_prt to merchant account
	virtual MerchantAccount *getMerchant(const unsigned long id) = 0;
//	virtual std::weak_ptr<MerchantAccount> getMerchant(const std::string &account) = 0;

	/// @brief Get a customer account
	/// @param id or account: the customer account's id or account
	/// @return a weak_prt to customer account
	virtual CustomerAccount *getCustomer(const unsigned long id) = 0;
//	virtual std::weak_ptr<CustomerAccount> getCustomer(const std::string &account) = 0;

	/// @brief Send verification code to merchant or customer via authentication carrier
	/// @param codeSendToWhere: the location where authentication carrier send verification code
//	virtual void merchantRequestForVerificationCode(const std::string &codeSendToWhere) = 0;
//	virtual void customerRequestForVerificationCode(const std::string &codeSendToWhere) = 0;

	/// @brief Check password for account
	/// @param account: the account for account
	/// @param password: the password for account
	/// @return a weak_prt to account who pass the authentication
//	virtual std::weak_ptr<MerchantAccount> merchantAuthentication(std::string &account, std::string &password) = 0;
//	virtual std::weak_ptr<CustomerAccount> customerAuthentication(std::string &account, std::string &password) = 0;

	/// @brief Load an account from data source
	/// @param id: the id for account
	/// @return a weak_prt to account whose loaded
//	virtual std::weak_ptr<MerchantAccount> getOrLoadMerchant(unsigned long id) = 0;
//	virtual std::weak_ptr<CustomerAccount> getOrLoadCustomer(unsigned long id) = 0;

//	virtual void updateServiceTypeFor(std::string merchant, std::list<std::string> applianceTypes, double maxDistance) = 0;
};


#endif //HARSERVER_ACCOUNTMANAGERSUBJECT_H
