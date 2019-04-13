//
// Created by gaojian on 19-3-24.
//

#include "MerchantFactory.h"
#include "Factories/DataSource/DataSource.hpp"
#include "Factories/DataSource/MariaDB/DatabaseConnection.h"
#include "Factories/DataSource/SimulateDatabase/SimulateDatabase.h"
#include "Account/MerchantAccount.h"
#include "Account/MerchantServiceType.h"

using std::shared_ptr;					using std::make_shared;
using std::string;						using std::get;

std::shared_ptr<Account> MerchantFactory::loadAccount(unsigned long id)
{
	auto accountInfo = DataSourceFrom::getInstance().loadMerchant(id);
	shared_ptr<MerchantAccount> merchant = make_shared<MerchantAccount>(id, get<0>(accountInfo), get<1>(accountInfo));
	return merchant;
}

std::shared_ptr<Account> MerchantFactory::loadAccountSpecific(std::string account, std::string password)
{
	unsigned long id = DataSourceFrom::getInstance().checkMerchantPasswordAndGetId(account, password);
	shared_ptr<MerchantAccount> merchant = make_shared<MerchantAccount>(id, account, password);

	// load service types
	auto serviceTypeDetail = DataSourceFrom::getInstance().queryMerchantServiceType(id);
	shared_ptr<MerchantServiceType> serviceType = make_shared<MerchantServiceType>(get<0>(serviceTypeDetail), get<1>(serviceTypeDetail));
	merchant->loadServiceType(serviceType);

	//TODO: load contact info
	return merchant;
}

void MerchantFactory::createAccountSpecific(std::string account, std::string password)
{
	DataSourceFrom::getInstance().createMerchantAccount(account, password);
}

void MerchantFactory::updateAccountSpecific(std::string account, std::string password)
{
	DataSourceFrom::getInstance().updateMerchantAccountPassword(account, password);
}