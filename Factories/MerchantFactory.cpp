//
// Created by gaojian on 19-3-24.
//

#include "MerchantFactory.h"
#include "Factories/DataSource/DataSource.hpp"
#include "Factories/DataSource/Postgres/PostgresConnection.h"
#include "Factories/DataSource/SimulateDatabase/SimulateDatabase.h"
#include "Account/MerchantAccount.h"
#include "Account/MerchantServiceType.h"
#include "Errors/QueryResultEmptyError.hpp"

using std::shared_ptr;					using std::make_shared;
using std::string;						using std::get;

std::shared_ptr<Account> MerchantFactory::loadAccount(unsigned long id)
{
	auto accountInfo = DATA_SOURCE_FROM::getInstance().loadAccount(id);
	shared_ptr<MerchantAccount> merchant = make_shared<MerchantAccount>(id, get<0>(accountInfo), get<1>(accountInfo));
	return merchant;
}

void MerchantFactory::persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant)
{
	DATA_SOURCE_FROM::getInstance().updateSupportedServices(merchant.lock()->id(), merchant.lock()->supportedServiceType().lock()->supportApplianceType(), merchant.lock()->supportedServiceType().lock()->maxRepairDistance());
}

std::shared_ptr<Account> MerchantFactory::loadAccountSpecific(std::string account, std::string password)
{
	unsigned long id = DATA_SOURCE_FROM::getInstance().checkPasswordAndGetId(account, password);
	shared_ptr<MerchantAccount> merchant = make_shared<MerchantAccount>(id, account, password);

	// load service types
	try
	{
		auto serviceTypeDetail = DATA_SOURCE_FROM::getInstance().queryMerchantServiceType(id);
		shared_ptr<MerchantServiceType> serviceType = make_shared<MerchantServiceType>(get<0>(serviceTypeDetail),
																					   get<1>(serviceTypeDetail));
		merchant->loadServiceType(serviceType);
	}
	catch (QueryResultEmptyError&)
	{}

	//TODO: load contact info
	return merchant;
}

void MerchantFactory::createAccountSpecific(std::string account, std::string password)
{
	DATA_SOURCE_FROM::getInstance().createMerchantAccount(account, password);
}

void MerchantFactory::updateAccountSpecific(std::string account, std::string password)
{
	DATA_SOURCE_FROM::getInstance().updateAccountPassword(account, password);
}