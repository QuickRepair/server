//
// Created by 17271 on 2019/9/24.
//

#ifndef HARSERVER_TOKENMANAGERSUBJECT_H
#define HARSERVER_TOKENMANAGERSUBJECT_H

class Account;

class TokenManagerSubject {
public:
	virtual ~TokenManagerSubject() = 0;

	virtual unsigned long newToken(Account *account) = 0;
	virtual bool tokenExist(unsigned long tokenId) = 0;
};

#endif //HARSERVER_TOKENMANAGERSUBJECT_H
