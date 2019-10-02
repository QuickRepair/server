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
	std::shared_ptr<Account> readAccount(std::string account, std::string password) override;
	std::shared_ptr<Account> createOrUpdateAccount(std::string account, std::string password) override;

	/// @brief Call this function to persistence merchant service types
	/// @param merchant: a weak_ptr to related merchant
//	void persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant);
};

#endif //HARSERVER_MERCHANTACCOUNTFACTORY_H
