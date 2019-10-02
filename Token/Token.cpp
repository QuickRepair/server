//
// Created by 17271 on 2019/9/24.
//

#include "Token.h"

Token::Token(unsigned long tokenId, unsigned long accountId)
	: m_tokenId{tokenId}, m_accoundId{accountId}
{}

unsigned long Token::getTokenId() const
{
	return m_tokenId;
}

unsigned long Token::getAssociatedAccountId() const
{
	return m_accoundId;
}
