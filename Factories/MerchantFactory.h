//
// Created by gaojian on 19-3-24.
//

#ifndef HARSERVER_MERCHANTACCOUNTFACTORY_H
#define HARSERVER_MERCHANTACCOUNTFACTORY_H

#include "AccountFactory.h"

class MerchantAccount;

class MerchantFactory : public AccountFactory {
public:
	~MerchantFactory() override = default;

	/// @override
	std::shared_ptr<Account> loadAccount(unsigned long id) override;

	/// @brief Call this function to persistence merchant service types
	/// @param merchant: a weak_ptr to related merchant
	void persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant);

protected:
	/// @override
	std::shared_ptr<Account> loadAccountSpecific(std::string account, std::string password) override;
	void createAccountSpecific(std::string account, std::string password) override;
	void updateAccountSpecific(std::string accout, std::string password) override;
};


#endif //HARSERVER_MERCHANTACCOUNTFACTORY_H
