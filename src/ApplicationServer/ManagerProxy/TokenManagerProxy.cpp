//
// Created by 17271 on 2019/9/24.
//

#include "TokenManagerProxy.h"
#include "ManagerProxy/Subjects/Managers/TokenManager.h"

TokenId TokenManagerProxy::newToken(Account *account)
{
	return TokenManager::getInstance().newToken(account);
}

bool TokenManagerProxy::tokenMatches(TokenId tokenId)
{
	return TokenManager::getInstance().tokenMatches(tokenId);
}
