//
// Created by gaojian on 19-4-25.
//

#include "AccountManagerProxy.h"
#include "Managers/AccountManager.h"

void AccountManagerProxy::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
{
	AccountManager::getInstance().registerAuthenticationCarrier(carrier);
}

std::list<std::weak_ptr<MerchantAccount>> AccountManagerProxy::getMerchantList()
{
	return AccountManager::getInstance().getMerchantList();
}

std::weak_ptr<MerchantAccount> AccountManagerProxy::getMerchant(const unsigned long id)
{
	return AccountManager::getInstance().getMerchant(id);
}

std::weak_ptr<MerchantAccount> AccountManagerProxy::getMerchant(const std::string &account)
{
	return AccountManager::getInstance().getMerchant(account);
}

std::weak_ptr<CustomerAccount> AccountManagerProxy::getCustomer(const unsigned long id)
{
	return AccountManager::getInstance().getCustomer(id);
}

std::weak_ptr<CustomerAccount> AccountManagerProxy::getCustomer(const std::string &account)
{
	return AccountManager::getInstance().getCustomer(account);
}

void AccountManagerProxy::merchantRequestForVerificationCode(const std::string &codeSendToWhere)
{
	AccountManager::getInstance().merchantRequestForVerificationCode(codeSendToWhere);
}

void AccountManagerProxy::customerRequestForVerificationCode(const std::string &codeSendToWhere)
{
	AccountManager::getInstance().customerRequestForVerificationCode(codeSendToWhere);
}

std::weak_ptr<MerchantAccount> AccountManagerProxy::merchantAuthentication(std::string &account, std::string &password)
{
	return AccountManager::getInstance().merchantAuthentication(account, password);
}

std::weak_ptr<CustomerAccount> AccountManagerProxy::customerAuthentication(std::string &account, std::string &password)
{
	return AccountManager::getInstance().customerAuthentication(account, password);
}

std::weak_ptr<MerchantAccount> AccountManagerProxy::getOrLoadMerchant(unsigned long id)
{
	return AccountManager::getInstance().getOrLoadMerchant(id);
}

std::weak_ptr<CustomerAccount> AccountManagerProxy::getOrLoadCustomer(unsigned long id)
{
	return AccountManager::getInstance().getOrLoadCustomer(id);
}
