//
// Created by gaojian on 19-3-24.
//

#include "AccountFactory.h"
#include "Errors/AccountAlreadyExistError.hpp"
#include <tuple>
#include <list>

AccountFactory::~AccountFactory()
{}

/*std::shared_ptr<Account> AccountFactory::createOrUpdateAccount(std::string account, std::string password)
{
	// create or update
	try {
		createAccountSpecific(account, password);
	} catch (AccountAlreadyExistError &e) {
		updateAccountSpecific(account, password);
	}

	// load user
	return readAccount(account, password);
}*/
