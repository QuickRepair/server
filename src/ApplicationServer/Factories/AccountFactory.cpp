//
// Created by gaojian on 19-3-24.
//

#include "AccountFactory.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include "Errors/AccountAlreadyExistError.hpp"
#include "types.h"
#include "Account/Account.h"
#include "DataSource/DataSource.hpp"

using std::make_unique;					using std::string;
using std::unique_ptr;					using std::list;
using std::shared_ptr;					using std::unordered_map;

/// AccountFactory
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
	return loadAccount(account, password);
}*/


/// CustomerFactory
//std::shared_ptr<Account> CustomerFactory::loadAccount(unsigned long id)
//{
//	auto accountInfo = DATA_SOURCE_FROM::getInstance().loadAccount(id);
//	shared_ptr<CustomerAccount> customer = make_shared<CustomerAccount>(id, get<0>(accountInfo), get<1>(accountInfo));
//	return customer;
//}

std::unique_ptr<Account> CustomerFactory::loadAccount(std::string account, std::string password)
{
	AccountId id = DataSource::getDataAccessInstance()->checkPasswordAndGetId(account, password);
	return make_unique<CustomerAccount>(id);
}

std::unique_ptr<Account> CustomerFactory::createOrUpdateAccount(std::string account, std::string password)
{
	DataSource::getDataAccessInstance()->createOrUpdatePasswordForCustomerAccount(account, password);
	return loadAccount(account, password);
}

void CustomerFactory::buildAccountConnection(Account *account)
{
	OrderManagerProxy orderManagerProxy;
	CustomerAccount *customer = dynamic_cast<CustomerAccount *>(account);
	customer->m_orders = orderManagerProxy.loadAccountOrders(customer->id());
}


/// MerchantFactory
//void MerchantFactory::persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant)
//{
//	DataSource::getDataAccessInstance()->updateSupportedServices(merchant.lock()->id(), merchant.lock()->supportedServiceType()->supportApplianceType(), merchant.lock()->supportedServiceType()->maxRepairDistance());
//}

std::unique_ptr<Account> MerchantFactory::loadAccount(std::string account, std::string password)
{
	AccountId id = DataSource::getDataAccessInstance()->checkPasswordAndGetId(account, password);
	unique_ptr<MerchantAccount> merchant = make_unique<MerchantAccount>(id);
	merchant->m_serviceType = make_unique<MerchantService>(merchant.get());
	return merchant;

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

}

std::unique_ptr<Account> MerchantFactory::createOrUpdateAccount(std::string account, std::string password)
{
	DataSource::getDataAccessInstance()->createOrUpdatePasswordForMerchantAccount(account, password);
	return loadAccount(account, password);
}

void MerchantFactory::buildAccountConnection(Account *account)
{
	OrderManagerProxy orderManagerProxy;
	MerchantAccount *merchant = dynamic_cast<MerchantAccount *>(account);
	merchant->m_orders = orderManagerProxy.loadAccountOrders(merchant->id());
}

std::unordered_map<AccountId, std::unique_ptr<MerchantAccount>> MerchantFactory::loadAllMerchants()
{
	unordered_map<AccountId, unique_ptr<MerchantAccount>> merchants;
	list<AccountId> id = DataSource::getDataAccessInstance()->getAllMerchants();
	for (auto &i : id)
		merchants[i] = make_unique<MerchantAccount>(i);
	return std::move(merchants);
}
