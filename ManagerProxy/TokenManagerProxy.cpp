//
// Created by 17271 on 2019/9/24.
//

#include "TokenManagerProxy.h"
#include "Subjects/Managers/TokenManager.h"

unsigned long TokenManagerProxy::newToken(Account *account)
{
	return TokenManager::getInstance().newToken(account);
}

bool TokenManagerProxy::tokenExist(unsigned long tokenId)
{
	return TokenManager::getInstance().tokenExist(tokenId);
}
