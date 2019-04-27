//
// Created by gaojian on 19-4-27.
//

#ifndef HARSERVER_AUTHENTICATIONSAVER_H
#define HARSERVER_AUTHENTICATIONSAVER_H

#include "AuthenticationCarrier.h"

/// @brief Save last authentication code int the class
class AuthenticationSaver : public AuthenticationCarrier {
public:
	~AuthenticationSaver() override = default;
	long sendVerificationCode(std::string sendTo) override;

	long lastAuthenticationCode() const;

private:
	long m_code;
};


#endif //HARSERVER_AUTHENTICATIONSAVER_H
