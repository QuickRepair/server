//
// Created by 17271 on 2019/9/24.
//

#ifndef HARSERVER_TOKENMANAGERPROXY_H
#define HARSERVER_TOKENMANAGERPROXY_H

#include "Subjects/TokenManagerSubject.h"

class TokenManagerProxy : public TokenManagerSubject {
public:
	~TokenManagerProxy() override = default;

	unsigned long newToken(Account *account) override;
	bool tokenExist(unsigned long tokenId) override;
};

#endif //HARSERVER_TOKENMANAGERPROXY_H
