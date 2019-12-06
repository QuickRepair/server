//
// Created by 17271 on 2019/9/24.
//

#ifndef HARSERVER_TOKENMANAGERSUBJECT_H
#define HARSERVER_TOKENMANAGERSUBJECT_H

#include "types.h"

class Account;

class TokenManagerSubject {
public:
	virtual ~TokenManagerSubject() = 0;

	virtual TokenId newToken(Account *account) = 0;
	virtual bool tokenMatches(TokenId tokenId) = 0;
};

#endif //HARSERVER_TOKENMANAGERSUBJECT_H
