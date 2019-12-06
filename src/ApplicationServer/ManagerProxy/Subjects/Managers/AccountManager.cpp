//
// Created by gaojian on 19-2-26.
//

#include "AccountManager.h"
#include "Account/Account.h"
#include "DataSource/DataSource.hpp"
#include "Factories/AccountFactory.h"
#include <algorithm>

using std::make_unique;					using std::list;
using std::unique_ptr;

AccountManager::AccountManager()
{
	// load all merchants
	MerchantFactory factory;
	m_merchantAccounts = factory.loadAllMerchants();
}

AccountManager &AccountManager::getInstance()
{
	static AccountManager instance;
	return instance;
}

Account *AccountManager::login(AccountType type, std::string account, std::string password)
{
	Account *accountPtr = nullptr;
	if (type == AccountType::Merchant)
	{
		MerchantFactory factory;
		unique_ptr<MerchantAccount> merchant(dynamic_cast<MerchantAccount *>(factory.loadAccount(account, password).release()));
		accountPtr = merchant.get();
		AccountId id = merchant->id();
		// no matter an existed id or new id, just move into map(replace one or create new pair)
		m_merchantAccounts[id] = std::move(merchant);
		factory.buildAccountConnection(accountPtr);
	}
	else if (type == AccountType::Customer)
	{
		CustomerFactory factory;
		unique_ptr<CustomerAccount> customer(dynamic_cast<CustomerAccount *>(factory.loadAccount(account, password).release()));
		accountPtr = customer.get();
		AccountId id = customer->id();
		m_customerAccounts[id] = std::move(customer);
		factory.buildAccountConnection(accountPtr);
	}
	return accountPtr;
}

/*void AccountManager::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
{
	m_authenticationCarrier = std::move(carrier);
}*/

std::list<MerchantAccount *> AccountManager::getMerchantList()
{
	// TODO: should limit distance
	list<MerchantAccount *> ret;
	for(auto &accounts : m_merchantAccounts)
		ret.push_back(accounts.second.get());
	return ret;
}

MerchantAccount *AccountManager::getMerchant(AccountId id)
{
	return m_merchantAccounts[id].get();
}

/*std::weak_ptr<MerchantAccount> AccountManager::getMerchant(const std::string &account)
{
	auto it = find_if(m_merchantAccountList.begin(), m_merchantAccountList.end(),
			[&account](shared_ptr<MerchantAccount> merchant) { return merchant->account() == account; });
	return it == m_merchantAccountList.end() ? nullptr : *it;
}*/

CustomerAccount *AccountManager::getCustomer(AccountId id)
{
	return m_customerAccounts[id].get() ? m_customerAccounts[id].get() : m_merchantAccounts[id].get();
}

void AccountManager::createOrUpdatePasswordForAccount(AccountType type, std::string account, std::string password)
{
	switch(type)
	{
		case AccountType::Merchant:
			DataSource::getDataAccessInstance()->createOrUpdatePasswordForMerchantAccount(account, password);
			break;
		case AccountType::Customer:
			DataSource::getDataAccessInstance()->createOrUpdatePasswordForCustomerAccount(account, password);
			break;
	}
}

/*std::weak_ptr<CustomerAccount> AccountManager::getCustomer(const std::string &account)
{
	auto it = find_if(m_customerAccountList.begin(), m_customerAccountList.end(),
			[&account](shared_ptr<CustomerAccount> customer) { return customer->account() == account; });
	return it == m_customerAccountList.end() ? nullptr : *it;
}*/

/*void AccountManager::merchantRequestForVerificationCode(const std::string &codeSendToWhere)
{
	// get verification code
	if(!m_authenticationCarrier)
		throw NoAuthenticationCarrierFound("Not registered an authentication carrier in account manager");
	long code = m_authenticationCarrier->sendVerificationCode(codeSendToWhere);

	// try to update/create account
	shared_ptr<MerchantAccount> account =
			dynamic_pointer_cast<MerchantAccount>(
					m_merchantFactory->createOrUpdateAccount(codeSendToWhere, std::to_string(code)));
	m_merchantAccountList.push_back(account);
}

void AccountManager::customerRequestForVerificationCode(const std::string &codeSendToWhere)
{
	// get verification code
	if(!m_authenticationCarrier)
		throw NoAuthenticationCarrierFound("Not registered an authentication carrier in account manager");
	long code = m_authenticationCarrier->sendVerificationCode(codeSendToWhere);

	// try to update/create account
	shared_ptr<CustomerAccount> account =
			dynamic_pointer_cast<CustomerAccount>(
					m_customerFactory->createOrUpdateAccount(codeSendToWhere, std::to_string(code)));
	m_customerAccountList.push_back(account);
}

std::weak_ptr<MerchantAccount> AccountManager::merchantAuthentication(std::string &account, std::string &password)
{
	if(isLoaded(account))
		return getMerchant(account);
	else
	{
		shared_ptr<MerchantAccount> merchant =
				dynamic_pointer_cast<MerchantAccount>(m_merchantFactory->readAccount(account, password));
		m_merchantAccountList.push_back(merchant);
		m_orderManagerProxy->loadAllOrderForAccount(merchant);
		return merchant;
	}
}

std::weak_ptr<CustomerAccount> AccountManager::customerAuthentication(std::string &account, std::string &password)
{
	if(isLoaded(account))
		return getCustomer(account);
	else
	{
		shared_ptr<CustomerAccount> customer =
				dynamic_pointer_cast<CustomerAccount>(m_customerFactory->loadAccount(account, password));
		m_customerAccountList.push_back(customer);
		m_orderManagerProxy->loadAllOrderForAccount(customer);
		return customer;
	}
}

std::weak_ptr<MerchantAccount> AccountManager::getOrLoadMerchant(unsigned long id)
{
	shared_ptr<MerchantAccount> merchant = getMerchant(id).lock();
	if(!merchant)
	{
		merchant = dynamic_pointer_cast<MerchantAccount>(m_merchantFactory->loadAccount(id));
		m_merchantAccountList.push_back(merchant);
	}
	return merchant;
}

std::weak_ptr<CustomerAccount> AccountManager::getOrLoadCustomer(unsigned long id)
{
	shared_ptr<CustomerAccount> customer = getCustomer(id).lock();
	if(!customer)
	{
		customer = dynamic_pointer_cast<CustomerAccount>(m_customerFactory->loadAccount(id));
		m_customerAccountList.push_back(customer);
	}
	return customer;
}*/

/*
void AccountManager::updateServiceTypeFor(std::string account, std::list<std::string> applianceTypes, double maxDistanc)
{
	auto merchant = getMerchant(account);
	merchant.lock()->updateSupportedService(applianceTypes, maxDistanc);
}

bool AccountManager::isLoaded(std::string &account)
{
	bool loaded{false};
	loaded = find_if(m_customerAccountList.begin(), m_customerAccountList.end(),
			[&account](shared_ptr<Account> customer){ return customer->account() == account; }) != m_customerAccountList.end();
	if(loaded)
		return loaded;
	loaded = find_if(m_merchantAccountList.begin(), m_merchantAccountList.end(),
			[&account](shared_ptr<Account> customer){ return customer->account() == account; }) != m_merchantAccountList.end();
	return loaded;
}*/
