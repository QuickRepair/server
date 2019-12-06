//
// Created by gaojian on 19-4-25.
//

#include "AccountManagerProxy.h"
#include "ManagerProxy/Subjects/Managers/AccountManager.h"

Account *AccountManagerProxy::login(AccountType type, std::string account, std::string password)
{
	return AccountManager::getInstance().login(type, account, password);
}

//void AccountManagerProxy::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
//{
//	AccountManager::getInstance().registerAuthenticationCarrier(carrier);
//}

std::list<MerchantAccount *> AccountManagerProxy::getMerchantList()
{
	return AccountManager::getInstance().getMerchantList();
}

MerchantAccount *AccountManagerProxy::getMerchant(const AccountId id)
{
	return AccountManager::getInstance().getMerchant(id);
}

//std::weak_ptr<MerchantAccount> AccountManagerProxy::getMerchant(const std::string &account)
//{
//	return AccountManager::getInstance().getMerchant(account);
//}

CustomerAccount *AccountManagerProxy::getCustomer(const AccountId id)
{
	return AccountManager::getInstance().getCustomer(id);
}

void AccountManagerProxy::createOrUpdatePasswordForAccount(AccountType type, std::string account, std::string password)
{
	return AccountManager::getInstance().createOrUpdatePasswordForAccount(type, account, password);
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
