//
// Created by gaojian on 19-3-24.
//

#include "AccountFactory.h"
#include "Database/DatabaseConnection.h"
#include <tuple>
#include <list>
#include "../Account/ContactInformation.h"
#include "../Errors/PasswordNotRightError.h"
#include "../Errors/NoSuchAnAccountError.h"

AccountFactory::~AccountFactory()
{}

std::shared_ptr<Account> AccountFactory::readAccount(std::string account, std::string password)
{
	return loadAccountSpecific(account, password);
}

std::shared_ptr<Account> AccountFactory::createOrUpdateAccount(std::string account, std::string password)
{
	// create or update
	try {
		createAccountSpecific(account, password);
	} catch (AccountAlreadyExistError &e) {
		updateAccountSpecific(account, password);
	}

	// load user
	return readAccount(account, password);
}
