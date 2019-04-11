//
// Created by gaojian on 19-3-19.
//

#include "AuthenticationToScreen.h"
#include <iostream>
#include <random>

using std::default_random_engine;

AuthenticationToScreen::~AuthenticationToScreen()
{}

long AuthenticationToScreen::sendVerificationCode(std::string sendTo)
{
	default_random_engine e;
	long code = e() % 1000000;
	std::cout << "Code to " << sendTo << " is " << code << std::endl;
	return code;
}
