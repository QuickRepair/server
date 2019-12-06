//
// Created by 17271 on 2019/9/24.
//

#ifndef HARSERVER_TOKEN_H
#define HARSERVER_TOKEN_H

#include "types.h"

class Account;

class Token {
public:
	Token(TokenId tokenId, Account *accountId);

	TokenId getTokenId() const;
	Account *getAssociatedAccount() const;

private:
	TokenId m_tokenId;
	Account *_accound;
};

#endif //HARSERVER_TOKEN_H
