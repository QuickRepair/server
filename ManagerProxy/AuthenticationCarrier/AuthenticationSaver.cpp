//
// Created by gaojian on 19-4-27.
//

#include "AuthenticationSaver.h"
#include <random>

using std::default_random_engine;

long AuthenticationSaver::sendVerificationCode(std::string sendTo)
{
	default_random_engine e;
	m_code = e() % 1000000;
	return m_code;
}

long AuthenticationSaver::lastAuthenticationCode() const
{
	return m_code;
}