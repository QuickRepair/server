//
// Created by gaojian on 19-2-26.
//

#include "AccountManager.hpp"
#include "../Account/Account.h"
#include "../Account/MerchantAccount.h"
#include "../Account/CustomerAccount.h"
#include "../Factories/UserFactory.hpp"

using std::shared_ptr;

AccountManager &AccountManager::getInstance()
{
	static AccountManager instance;
	return instance;
}

std::list<std::weak_ptr<MerchantAccount>> AccountManager::getMerchantList()
{
	std::list<std::weak_ptr<MerchantAccount>> ret;
	for(std::weak_ptr<MerchantAccount> accounts : m_merchantAccount)
		ret.push_back(accounts);
	return ret;
}

void AccountManager::registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier)
{
	m_authenticationCarrier = carrier;
}
