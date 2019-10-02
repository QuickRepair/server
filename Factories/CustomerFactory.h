//
// Created by gaojian on 19-3-24.
//

#ifndef HARSERVER_CUSTOMERFACTORY_H
#define HARSERVER_CUSTOMERFACTORY_H

#include "AccountFactory.h"

class CustomerFactory : public AccountFactory {
public:
	~CustomerFactory() override = default;
	/// @override
	std::shared_ptr<Account> readAccount(std::string account, std::string password) override;
	std::shared_ptr<Account> createOrUpdateAccount(std::string account, std::string password) override;
	/// @override
//	std::shared_ptr<Account> loadAccount(unsigned long id) override;
};

#endif //HARSERVER_CUSTOMERFACTORY_H
