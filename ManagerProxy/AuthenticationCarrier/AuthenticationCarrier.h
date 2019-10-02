//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_AUTHENTICATIONCARRIER_H
#define HARSERVER_AUTHENTICATIONCARRIER_H

#include <string>

/// @brief The interface of all authentication carriers
class AuthenticationCarrier {
public:
	virtual ~AuthenticationCarrier() = 0;

	/// @brief Send verification to given code
	/// @param sendTo: represent where does the code go
	/// @return the code just sent
	virtual unsigned sendVerificationCode(std::string sendTo) = 0;
};

#endif //HARSERVER_AUTHENTICATIONCARRIER_H
