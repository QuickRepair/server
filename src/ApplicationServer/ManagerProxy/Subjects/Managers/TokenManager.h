//
// Created by 17271 on 2019/9/27.
//

#ifndef HARSERVER_TOKENMANAGER_H
#define HARSERVER_TOKENMANAGER_H

#include "ManagerProxy/Subjects/TokenManagerSubject.h"
#include <unordered_map>

class TokenManager : public TokenManagerSubject {
public:
	~TokenManager() override = default;

	static TokenManager &getInstance();

	TokenId newToken(Account *account) override;
	bool tokenMatches(TokenId tokenId) override;

private:
	std::unordered_map<TokenId, Account *> m_tokenAccountPairs;
};


#endif //HARSERVER_TOKENMANAGER_H
