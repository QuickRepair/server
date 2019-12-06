//
// Created by 17271 on 2019/9/24.
//

#include "Token.h"

Token::Token(TokenId tokenId, Account *accountId)
	: m_tokenId{tokenId}, _accound{accountId}
{}

TokenId Token::getTokenId() const
{
	return m_tokenId;
}

Account * Token::getAssociatedAccount() const
{
	return _accound;
}
