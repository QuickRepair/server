//
// Created by gaojian on 19-3-24.
//

#ifndef HARSERVER_CUSTOMERFACTORY_H
#define HARSERVER_CUSTOMERFACTORY_H

#include "AccountFactory.h"

class CustomerFactory : public AccountFactory {
public:
	~CustomerFactory() override = default;
	std::shared_ptr<Account> loadAccount(unsigned long id) override;

protected:
	std::shared_ptr<Account> loadAccountSpecific(std::string account, std::string password) override;
	void createAccountSpecific(std::string account, std::string password) override;
	void updateAccountSpecific(std::string accout, std::string password) override;
};


#endif //HARSERVER_CUSTOMERFACTORY_H
