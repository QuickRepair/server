//
// Created by gaojian on 19-4-25.
//

#ifndef HARSERVER_ACCOUNTMANAGERPROXY_H
#define HARSERVER_ACCOUNTMANAGERPROXY_H

#include "ManagerProxy/Subjects/AccountManagerSubject.h"

class OrderManagerProxy;

/// @brief Proxy for singleton AccountManager
class AccountManagerProxy : public AccountManagerSubject {
public:
	AccountManagerProxy();
	~AccountManagerProxy() override = default;

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

	/// @override
	void updateServiceTypeFor(std::string merchant, std::list<std::string> applianceTypes, double maxDistanc) override;

private:
	std::shared_ptr<OrderManagerProxy> m_orderManagerProxy;
};


#endif //HARSERVER_ACCOUNTMANAGERPROXY_H
