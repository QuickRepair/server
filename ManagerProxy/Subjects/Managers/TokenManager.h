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

	unsigned long newToken(Account *account) override;
	bool tokenExist(unsigned long tokenId) override;

private:
	std::unordered_map<unsigned long, Account *> m_tokenAccountPairs;
};


#endif //HARSERVER_TOKENMANAGER_H
