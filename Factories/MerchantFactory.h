//
// Created by gaojian on 19-3-24.
//

#ifndef HARSERVER_MERCHANTACCOUNTFACTORY_H
#define HARSERVER_MERCHANTACCOUNTFACTORY_H

#include "AccountFactory.h"

class MerchantFactory : public AccountFactory {
public:
	~MerchantFactory() override = default;

protected:
	std::shared_ptr<Account> loadAccountSpecific(std::string account, std::string password) override;
	void createAccountSpecific(std::string account, std::string password) override;
	void updateAccountSpecific(std::string accout, std::string password) override;
};


#endif //HARSERVER_MERCHANTACCOUNTFACTORY_H
