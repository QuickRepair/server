//
// Created by gaojian on 19-2-26.
//

#ifndef HAR_ACCOUNTMANAGER_H
#define HAR_ACCOUNTMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "ManagerProxy/Subjects/AccountManagerSubject.h"

class Account;
class MerchantAccount;
class CustomerAccount;

/// @brief The singleton who actually holds accounts
/// @note: do NOT use this class, use AccountManagerProxy instead
class AccountManager : public AccountManagerSubject {
public:
	~AccountManager() override = default;

	/// @brief Get instance
	static AccountManager &getInstance();

	Account *loadAccount(Account::Type type, unsigned long id) override;
	void createOrUpdatePasswordForAccount(Account::Type type, std::string account, std::string password) override;
	unsigned long getAccountIdIfPasswordRight(std::string account, std::string password);
	/// @override
//	void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier) override;

	/// @override
	std::list<MerchantAccount *> getMerchantList() override;
	MerchantAccount *getMerchant(unsigned long id) override;
//	std::weak_ptr<MerchantAccount> getMerchant(const std::string &account) override;
	CustomerAccount *getCustomer(unsigned long id) override;
//	std::weak_ptr<CustomerAccount> getCustomer(const std::string &account) override;

	/// @override
//	void merchantRequestForVerificationCode(const std::string &codeSendToWhere) override;
//	void customerRequestForVerificationCode(const std::string &codeSendToWhere) override;

	/// @override
//	std::weak_ptr<MerchantAccount> merchantAuthentication(std::string &account, std::string &password) override;
//	std::weak_ptr<CustomerAccount> customerAuthentication(std::string &account, std::string &password) override;
//	std::weak_ptr<MerchantAccount> getOrLoadMerchant(unsigned long id) override;
//	std::weak_ptr<CustomerAccount> getOrLoadCustomer(unsigned long id) override;

	/// @override
//	void updateServiceTypeFor(std::string account, std::list<std::string> applianceTypes, double maxDistanc) override;

private:
	AccountManager() = default;

//	bool isLoaded(std::string &account);

//	std::shared_ptr<MerchantFactory> m_merchantFactory;
//	std::shared_ptr<CustomerFactory> m_customerFactory;
//	std::shared_ptr<OrderManagerProxy> m_orderManagerProxy;

	std::unordered_map<unsigned long, std::unique_ptr<MerchantAccount>> m_merchantAccounts;
	std::unordered_map<unsigned long, std::unique_ptr<CustomerAccount>> m_customerAccounts;
};

#endif //HAR_ACCOUNTMANAGER_H
