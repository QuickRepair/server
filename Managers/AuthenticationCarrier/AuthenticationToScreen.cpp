//
// Created by gaojian on 19-3-19.
//

#include "AuthenticationToScreen.h"
#include <iostream>

AuthenticationToScreen::~AuthenticationToScreen()
{}

long AuthenticationToScreen::sendVerificationCode(std::string sendTo)
{
	long code = random() % 1000000;
	std::cout << "Code to " << sendTo << " is " << code << std::endl;
	return code;
}
