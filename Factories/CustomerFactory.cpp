//
// Created by gaojian on 19-3-24.
//

#include "CustomerFactory.h"
#include "Database/DatabaseConnection.h"

using std::shared_ptr;					using std::make_shared;
using std::string;						using std::get;

std::shared_ptr<Account> CustomerFactory::loadAccount(unsigned long id)
{
	auto accountInfo = DatabaseConnection::getInstance().loadCustomer(id);
	shared_ptr<CustomerAccount> customer = make_shared<CustomerAccount>(id, get<0>(accountInfo), get<1>(accountInfo));
	return customer;
}

std::shared_ptr<Account> CustomerFactory::loadAccountSpecific(std::string account, std::string password)
{
	unsigned long id = DatabaseConnection::getInstance().checkCustomerPasswordAndGetId(account, password);
	shared_ptr<CustomerAccount> customer = make_shared<CustomerAccount>(id, account, password);

	//TODO: load contact info
	return customer;
}

void CustomerFactory::createAccountSpecific(std::string account, std::string password)
{
	DatabaseConnection::getInstance().createCustomerAccount(account, password);
}

void CustomerFactory::updateAccountSpecific(std::string account, std::string password)
{
	DatabaseConnection::getInstance().updateCustomerAccountPassword(account, password);
}