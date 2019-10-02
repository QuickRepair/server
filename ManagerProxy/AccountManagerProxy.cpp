//
// Created by gaojian on 19-4-25.
//

#include "AccountManagerProxy.h"
#include "Subjects/Managers/AccountManager.h"

Account *AccountManagerProxy::loadAccount(Account::Type type, unsigned long id)
{
	return AccountManager::getInstance().loadAccount(type, id);
}

//void AccountManagerProxy::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
//{
//	AccountManager::getInstance().registerAuthenticationCarrier(carrier);
//}

std::list<MerchantAccount *> AccountManagerProxy::getMerchantList()
{
	return AccountManager::getInstance().getMerchantList();
}

MerchantAccount *AccountManagerProxy::getMerchant(const unsigned long id)
{
	return AccountManager::getInstance().getMerchant(id);
}

//std::weak_ptr<MerchantAccount> AccountManagerProxy::getMerchant(const std::string &account)
//{
//	return AccountManager::getInstance().getMerchant(account);
//}

CustomerAccount *AccountManagerProxy::getCustomer(const unsigned long id)
{
	return AccountManager::getInstance().getCustomer(id);
}

void AccountManagerProxy::createOrUpdatePasswordForAccount(Account::Type type, std::string account, std::string password)
{
	return AccountManager::getInstance().createOrUpdatePasswordForAccount(type, account, password);
}

unsigned long AccountManagerProxy::getAccountIdIfPasswordRight(std::string account, std::string password)
{
	return AccountManager::getInstance().getAccountIdIfPasswordRight(account, password);
}

//std::weak_ptr<CustomerAccount> AccountManagerProxy::getCustomer(const std::string &account)
//{
//	return AccountManager::getInstance().getCustomer(account);
//}

/*void AccountManagerProxy::merchantRequestForVerificationCode(const std::string &codeSendToWhere)
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

void AccountManagerProxy::updateServiceTypeFor(std::string merchant, std::list<std::string> applianceTypes, double maxDistanc)
{
	return AccountManager::getInstance().updateServiceTypeFor(merchant, applianceTypes, maxDistanc);
}*/
