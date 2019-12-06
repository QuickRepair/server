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

TokenId TokenManager::newToken(Account *account)
{
	// TODO update token algorithm
	std::size_t token = std::hash<AccountId>{}(account->id());
	m_tokenAccountPairs[token] = account;
	return token;
}

bool TokenManager::tokenMatches(TokenId tokenId)
{
	return m_tokenAccountPairs.find(tokenId) != m_tokenAccountPairs.end();
}
