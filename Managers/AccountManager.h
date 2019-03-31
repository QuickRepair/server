//
// Created by gaojian on 19-2-26.
//

#ifndef HAR_ACCOUNTMANAGER_H
#define HAR_ACCOUNTMANAGER_H

#include <list>
#include <memory>
#include <string>

class MerchantAccount;
class CustomerAccount;
class AccountFactory;
class AuthenticationCarrier;

class AccountManager {
public:
	static AccountManager &getInstance();

	void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier);

	std::list<std::weak_ptr<MerchantAccount>> getMerchantList();
	std::weak_ptr<MerchantAccount> getMerchant(unsigned long id);
	std::weak_ptr<MerchantAccount> getMerchant(std::string &account);
	std::weak_ptr<CustomerAccount> getCustomer(unsigned long id);
	std::weak_ptr<CustomerAccount> getCustomer(std::string &account);

	void merchantRequestForVerificationCode(std::string &codeSendToWhere);
	void customerRequestForVerificationCode(std::string &codeSendToWhere);

	std::weak_ptr<MerchantAccount> merchantAuthentication(std::string &account, std::string &password);
	std::weak_ptr<CustomerAccount> customerAuthentication(std::string &account, std::string &password);
	std::weak_ptr<MerchantAccount> loadMerchant(unsigned long id);
	std::weak_ptr<CustomerAccount> loadCustomer(unsigned long id);

private:
	AccountManager();

	std::shared_ptr<AccountFactory> m_merchantFactory;
	std::shared_ptr<AccountFactory> m_customerFactory;

	std::list<std::shared_ptr<MerchantAccount>> m_merchantAccountList;
	std::list<std::shared_ptr<CustomerAccount>> m_customerAccountList;

	std::shared_ptr<AuthenticationCarrier> m_authenticationCarrier;
};

#endif //HAR_ACCOUNTMANAGER_H
