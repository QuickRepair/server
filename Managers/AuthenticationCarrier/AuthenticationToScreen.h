//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_AUTHENTICATIONTOSCREEN_H
#define HARSERVER_AUTHENTICATIONTOSCREEN_H

#include "AuthenticationCarrier.h"

class AuthenticationToScreen : public AuthenticationCarrier {
public:
	~AuthenticationToScreen() override;
	long sendVerificationCode(std::string sendTo) override;
};


#endif //HARSERVER_AUTHENTICATIONTOSCREEN_H