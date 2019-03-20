//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_AUTHENTICATIONCARRIER_H
#define HARSERVER_AUTHENTICATIONCARRIER_H

#include <string>

class AuthenticationCarrier {
public:
	virtual ~AuthenticationCarrier() = 0;
	virtual long sendVerificationCode(std::string sendTo) = 0;
};


#endif //HARSERVER_AUTHENTICATIONCARRIER_H
