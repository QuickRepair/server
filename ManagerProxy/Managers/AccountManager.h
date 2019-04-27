//
// Created by gaojian on 19-2-26.
//

#ifndef HAR_ACCOUNTMANAGER_H
#define HAR_ACCOUNTMANAGER_H

#include <list>
#include <memory>
#include <string>
#include "ManagerProxy/Subjects/AccountManagerSubject.h"

class MerchantAccount;
class CustomerAccount;
class AccountFactory;
class AuthenticationCarrier;
class Order;

/// @brief The singleton who actually holds accounts
/// @note: do NOT use this class, use AccountManagerProxy instead
class AccountManager : public AccountManagerSubject {
public:

	~AccountManager() override = default;

	/// @brief Get instance
	static AccountManager &getInstance();

	/// @override
	void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier) override;

	/// @override
	std::list<std::weak_ptr<MerchantAccount>> getMerchantList() override;
	std::weak_ptr<MerchantAccount> getMerchant(const unsigned long id) override;
	std::weak_ptr<MerchantAccount> getMerchant(const std::string &account) override;
	std::weak_ptr<CustomerAccount> getCustomer(const unsigned long id) override;
	std::weak_ptr<CustomerAccount> getCustomer(const std::string &account) override;

	/// @override
	void merchantRequestForVerificationCode(const std::string &codeSendToWhere) override;
	void customerRequestForVerificationCode(const std::string &codeSendToWhere) override;

	/// @override
	std::weak_ptr<MerchantAccount> merchantAuthentication(std::string &account, std::string &password) override;
	std::weak_ptr<CustomerAccount> customerAuthentication(std::string &account, std::string &password) override;
	std::weak_ptr<MerchantAccount> getOrLoadMerchant(unsigned long id) override;
	std::weak_ptr<CustomerAccount> getOrLoadCustomer(unsigned long id) override;

private:
	AccountManager();

	std::shared_ptr<AccountFactory> m_merchantFactory;
	std::shared_ptr<AccountFactory> m_customerFactory;

	std::list<std::shared_ptr<MerchantAccount>> m_merchantAccountList;
	std::list<std::shared_ptr<CustomerAccount>> m_customerAccountList;

	std::shared_ptr<AuthenticationCarrier> m_authenticationCarrier;
};

#endif //HAR_ACCOUNTMANAGER_H