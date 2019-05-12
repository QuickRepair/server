//
// Created by gaojian on 19-2-26.
//

#include "AccountManager.h"
#include "Account/Account.h"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "Factories/MerchantFactory.h"
#include "Factories/CustomerFactory.h"
#include "Errors/NoAuthenticationCarrierFound.hpp"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include <algorithm>

using std::shared_ptr;						using std::find_if;
using std::dynamic_pointer_cast;			using std::list;
using std::weak_ptr;						using std::make_shared;

AccountManager::AccountManager()
{
	m_merchantFactory = make_shared<MerchantFactory>();
	m_customerFactory = make_shared<CustomerFactory>();
	m_orderManagerProxy = make_shared<OrderManagerProxy>();
}

AccountManager &AccountManager::getInstance()
{
	static AccountManager instance;
	return instance;
}

void AccountManager::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
{
	m_authenticationCarrier = std::move(carrier);
}

std::list<std::weak_ptr<MerchantAccount>> AccountManager::getMerchantList()
{
	list<weak_ptr<MerchantAccount>> ret;
	for(weak_ptr<MerchantAccount> accounts : m_merchantAccountList)
		ret.push_back(accounts);
	return ret;
}

std::weak_ptr<MerchantAccount> AccountManager::getMerchant(unsigned long id)
{
	auto it = find_if(m_merchantAccountList.begin(), m_merchantAccountList.end(),
					  [&id](shared_ptr<MerchantAccount> merchant) { return merchant->id() == id; });
	return it == m_merchantAccountList.end() ? nullptr : *it;
}

std::weak_ptr<MerchantAccount> AccountManager::getMerchant(const std::string &account)
{
	auto it = find_if(m_merchantAccountList.begin(), m_merchantAccountList.end(),
			[&account](shared_ptr<MerchantAccount> merchant) { return merchant->account() == account; });
	return it == m_merchantAccountList.end() ? nullptr : *it;
}

std::weak_ptr<CustomerAccount> AccountManager::getCustomer(unsigned long id)
{
	auto it = find_if(m_customerAccountList.begin(), m_customerAccountList.end(),
			[&id](shared_ptr<CustomerAccount> customer) { return customer->id() == id; });
	return it == m_customerAccountList.end() ? nullptr : *it;
}

std::weak_ptr<CustomerAccount> AccountManager::getCustomer(const std::string &account)
{
	auto it = find_if(m_customerAccountList.begin(), m_customerAccountList.end(),
			[&account](shared_ptr<CustomerAccount> customer) { return customer->account() == account; });
	return it == m_customerAccountList.end() ? nullptr : *it;
}

void AccountManager::merchantRequestForVerificationCode(const std::string &codeSendToWhere)
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
				dynamic_pointer_cast<CustomerAccount>(m_customerFactory->readAccount(account, password));
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
}