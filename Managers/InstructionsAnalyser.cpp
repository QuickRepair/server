//
// Created by gaojian on 19-3-19.
//

#include "InstructionsAnalyser.h"
#include "AccountManager.hpp"
#include "../Account/MerchantServiceType.h"

using std::string;					using std::cerr;
using std::endl;					using std::list;
using std::weak_ptr;

std::string InstructionsAnalyser::instructionFromMap(std::map<utility::string_t, utility::string_t> instruction)
{
	string replyMsg;

	if(instruction["type"] == "get_verification")				// get verification
		replyMsg = doGetVerification(instruction);
	else if(instruction.find("get_list") != instruction.end())	//get merchant/appliance type list
		replyMsg = doGetList(instruction);
	else
		std::cerr << "No such an instruction" << std::endl;

	return replyMsg;
}

std::string InstructionsAnalyser::instructionFromJson(web::json::value json)
{
	string replyMsg;

	web::json::object object = json.as_object();
	string type = object.at("type").as_string();
	if(type == "login")					// login
		replyMsg = doLogin(object);

	return replyMsg;
}

std::string InstructionsAnalyser::doGetVerification(std::map<utility::string_t, utility::string_t> &instruction)
{
	string replyMsg;

	try
	{
		if (instruction["account_type"] == "merchant")
			AccountManager::getInstance().requestForVerificationCode<MerchantAccount>(instruction["account"]);
		else if (instruction["account_type"] == "customer")
			AccountManager::getInstance().requestForVerificationCode<CustomerAccount>(instruction["account"]);
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return replyMsg;
}

std::string InstructionsAnalyser::doLogin(web::json::object &object)
{
	web::json::value retJson;

	try
	{
		string account = object.at("account").as_string();
		string password = object.at("password").as_string();
		string accountType = object.at("account_type").as_string();
		if(accountType == "merchant")
			AccountManager::getInstance().accountAuthentication<MerchantAccount>(account, password);
		else if(accountType == "customer")
			AccountManager::getInstance().accountAuthentication<CustomerAccount>(account, password);
		retJson["login_result"] = web::json::value::string("success");
	}
	catch (NoSuchAnAccountError &e)
	{
		retJson["login_result"] = web::json::value::string("no such an account");
	}
	catch (PasswordNotRightError &e)
	{
		retJson["login_result"] = web::json::value::string("wrong password");
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

std::string InstructionsAnalyser::doGetList(std::map<utility::string_t, utility::string_t> &instruction)
{
	web::json::value retJson;

	if(instruction["get_list"] == "merchant_list")
	{
		list<weak_ptr<MerchantAccount>> merchants = AccountManager::getInstance().getMerchantList();
		web::json::value array;
		auto it = merchants.begin();
		for(unsigned i = 0; it != merchants.end(); ++i, ++it)
			array[i] = web::json::value(it->lock()->userName());
		retJson["merchant_list"] = array;
	}
	else if(instruction["get_list"] == "appliance_type")
	{
		list<weak_ptr<MerchantAccount>> merchantList = AccountManager::getInstance().getMerchantList();
		web::json::value array;
		for(auto merchant : merchantList)
		{
			list<string> typeList = merchant.lock()->supportedServiceType().lock()->supportApplianceType();
			auto it = typeList.begin();
			for(unsigned i = 0; it != typeList.end(); ++i, ++it)
				array[i] = web::json::value(*it);
		}
		retJson["appliance_list"] = array;
	}

	return retJson.serialize();
}
