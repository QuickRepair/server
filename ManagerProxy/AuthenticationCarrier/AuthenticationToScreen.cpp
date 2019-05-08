//
// Created by gaojian on 19-3-19.
//

#include "AuthenticationToScreen.h"
#include <iostream>
#include <random>
#include <iomanip>

using std::random_device;
using std::default_random_engine;			using std::uniform_int_distribution;
using std::setw;							using std::setfill;

unsigned AuthenticationToScreen::sendVerificationCode(std::string sendTo)
{
	random_device r;
	default_random_engine e(r());
	uniform_int_distribution<unsigned> distribution(0, 999999);
	unsigned code = distribution(e);
	std::cout << "Code to " << sendTo << " is " << setfill('0') << setw(6) << code << std::endl;
	return code;
}
