//
// Created by 17271 on 2019/9/27.
//

#include "TokenManager.h"
#include "Account/Account.h"
#include <functional>

TokenManager& TokenManager::getInstance()
{
	static TokenManager instance;
	return instance;
}

unsigned long TokenManager::newToken(Account *account)
{
	// TODO update token algorithm
	std::size_t token = std::hash<unsigned long>{}(account->id());
	m_tokenAccountPairs[token] = account;
	return token;
}

bool TokenManager::tokenExist(unsigned long tokenId)
{
	return m_tokenAccountPairs.contains(tokenId);
}
