//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_AUTHENTICATIONTOSCREEN_H
#define HARSERVER_AUTHENTICATIONTOSCREEN_H

#include "AuthenticationCarrier.h"

/// @brief Out put authentication code to terminal
class AuthenticationToScreen : public AuthenticationCarrier {
public:
	~AuthenticationToScreen() override = default;
	unsigned sendVerificationCode(std::string sendTo) override;
};


#endif //HARSERVER_AUTHENTICATIONTOSCREEN_H
