//
// Created by gaojian on 19-3-24.
//

#include "Account/CustomerAccount.h"
#include "CustomerFactory.h"
#include "DataSource/DataSource.hpp"

using std::shared_ptr;					using std::make_shared;
using std::string;						using std::get;

//std::shared_ptr<Account> CustomerFactory::loadAccount(unsigned long id)
//{
//	auto accountInfo = DATA_SOURCE_FROM::getInstance().loadAccount(id);
//	shared_ptr<CustomerAccount> customer = make_shared<CustomerAccount>(id, get<0>(accountInfo), get<1>(accountInfo));
//	return customer;
//}

std::shared_ptr<Account> CustomerFactory::readAccount(std::string account, std::string password)
{
	unsigned long id = DataSource::getDataAccessInstance()->checkPasswordAndGetId(account, password);
	return make_shared<CustomerAccount>(id);
}

std::shared_ptr<Account> CustomerFactory::createOrUpdateAccount(std::string account, std::string password)
{
	DataSource::getDataAccessInstance()->createOrUpdatePasswordForCustomerAccount(account, password);
	return readAccount(account, password);
}
