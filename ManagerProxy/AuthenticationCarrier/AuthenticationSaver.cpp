//
// Created by gaojian on 19-4-27.
//

#include "AuthenticationSaver.h"
#include <random>

using std::random_device;
using std::default_random_engine;			using std::uniform_int_distribution;

unsigned AuthenticationSaver::sendVerificationCode(std::string sendTo)
{
	random_device r;
	default_random_engine e(r());
	uniform_int_distribution<unsigned> distribution(0, 999999);
	m_code = distribution(e);
	return m_code;
}

unsigned AuthenticationSaver::lastAuthenticationCode() const
{
	return m_code;
}