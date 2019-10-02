//
// Created by 17271 on 2019/9/24.
//

#ifndef HARSERVER_TOKEN_H
#define HARSERVER_TOKEN_H

class Token {
public:
	Token(unsigned long tokenId, unsigned long accountId);

	unsigned long getTokenId() const;
	unsigned long getAssociatedAccountId() const;

private:
	unsigned long m_tokenId;
	unsigned long m_accoundId;
};

#endif //HARSERVER_TOKEN_H
