//
// Created by gaojian on 19-3-19.
//

#include "InstructionsAnalyser.h"
#include "AccountManager.hpp"

void InstructionsAnalyser::instructionFromMap(std::map<utility::string_t, utility::string_t> instruction)
{
	// get verification
	if(instruction.find("verification") != instruction.end() && instruction.find("account_type") != instruction.end())
	{
		if(instruction["account_type"] == "merchant")
			AccountManager::getInstance().requestForVerificationCode<MerchantAccount>(instruction["verification"]);
		else if(instruction["account_type"] == "customer")
			AccountManager::getInstance().requestForVerificationCode<CustomerAccount>(instruction["verification"]);
	}
	else
		std::cerr << "No such an instruction" << std::endl;
}
