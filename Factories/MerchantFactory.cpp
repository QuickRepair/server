//
// Created by gaojian on 19-3-24.
//

#include "MerchantFactory.h"
#include "DataSource/DataSource.hpp"
#include "Account/MerchantAccount.h"
#include "Account/MerchantService.h"
#include "Errors/QueryResultEmptyError.hpp"

using std::shared_ptr;					using std::make_shared;
using std::string;						using std::get;

//void MerchantFactory::persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant)
//{
//	DataSource::getDataAccessInstance()->updateSupportedServices(merchant.lock()->id(), merchant.lock()->supportedServiceType()->supportApplianceType(), merchant.lock()->supportedServiceType()->maxRepairDistance());
//}

std::shared_ptr<Account> MerchantFactory::readAccount(std::string account, std::string password)
{
	unsigned long id = DataSource::getDataAccessInstance()->checkPasswordAndGetId(account, password);
	shared_ptr<MerchantAccount> merchant = make_shared<MerchantAccount>(id);

	// load service types
	/*try
	{
		auto serviceTypeDetail = DataSource::getDataAccessInstance()->queryMerchantServiceType(id);
		unique_ptr<MerchantServiceType> serviceType = make_unique<MerchantServiceType>(get<0>(serviceTypeDetail),
																					   get<1>(serviceTypeDetail));
		merchant->loadServiceType(serviceType);
	}
	catch (QueryResultEmptyError&)
	{}*/

	return merchant;
}

std::shared_ptr<Account> MerchantFactory::createOrUpdateAccount(std::string account, std::string password)
{
	DataSource::getDataAccessInstance()->createOrUpdatePasswordForMerchantAccount(account, password);
	return readAccount(account, password);
}
