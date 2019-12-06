//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ACCOUNTMANAGERPROXY_H
#define HARSERVER_ACCOUNTMANAGERPROXY_H

#include "ManagerProxy/Subjects/AccountManagerSubject.h"

/// @brief Proxy for singleton AccountManager
class AccountManagerProxy : public AccountManagerSubject {
public:
	AccountManagerProxy() = default;
	~AccountManagerProxy() override = default;

	Account *login(AccountType type, std::string account, std::string password) override;
	void createOrUpdatePasswordForAccount(AccountType type, std::string account, std::string password) override;
	/// @override
//	void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier) override;

	/// @override
	std::list<MerchantAccount *> getMerchantList() override;
	MerchantAccount *getMerchant(AccountId id) override;
//	std::weak_ptr<MerchantAccount> getMerchant(const std::string &account) override;
	CustomerAccount *getCustomer(AccountId id) override;
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
//	void updateServiceTypeFor(std::string merchant, std::list<std::string> applianceTypes, double maxDistanc) override;
};


#endif //HARSERVER_ACCOUNTMANAGERPROXY_H
