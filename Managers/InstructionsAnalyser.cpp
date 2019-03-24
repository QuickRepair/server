//
// Created by gaojian on 19-3-19.
//

#include "InstructionsAnalyser.h"
#include "AccountManager.h"
#include "../Account/MerchantServiceType.h"
#include "../Account/MerchantAccount.h"
#include "../Errors/DatabaseInternalError.h"
#include "../Errors/NoSuchAnAccountError.h"
#include "../Errors/PasswordNotRightError.h"

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
	else if(type == "submit_service_type")
		replyMsg = doUpdateServiceType(object);

	return replyMsg;
}

std::string InstructionsAnalyser::doGetVerification(std::map<utility::string_t, utility::string_t> &instruction)
{
	string replyMsg;

	try
	{
		if (instruction["account_type"] == "merchant")
			AccountManager::getInstance().merchantRequestForVerificationCode(instruction["account"]);
		else if (instruction["account_type"] == "customer")
			AccountManager::getInstance().customerRequestForVerificationCode(instruction["account"]);
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
			AccountManager::getInstance().merchantAuthentication(account, password);
		else if(accountType == "customer")
			AccountManager::getInstance().customerAuthentication(account, password);
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

	if(instruction["get_list"] == "origin_service_type")	// get origin service types
	{
		weak_ptr<MerchantAccount> account = AccountManager::getInstance().getMerchant(instruction["account"]);
		weak_ptr<MerchantServiceType> serviceType = account.lock()->supportedServiceType();
		web::json::value array;
		list<string> supportList = serviceType.lock()->supportApplianceType();
		auto it = supportList.begin();
		for(unsigned i = 0; it != supportList.end(); ++i, ++it)
			array[i] = web::json::value(*it);
		retJson["support_appliance_type"] = array;
		retJson["max_distance"] = web::json::value(serviceType.lock()->maxRepairDistance());
	}
	else if(instruction["get_list"] == "merchant_list")		//get merchant list
	{
		list<weak_ptr<MerchantAccount>> merchants = AccountManager::getInstance().getMerchantList();
		web::json::value array;
		auto it = merchants.begin();
		for(unsigned i = 0; it != merchants.end(); ++i, ++it)
			array[i] = web::json::value(it->lock()->account());
		retJson["merchant_list"] = array;
	}
	else if(instruction["get_list"] == "appliance_type")	//get appliance type list
	{
		list<weak_ptr<MerchantAccount>> merchantList = AccountManager::getInstance().getMerchantList();
		web::json::value array;
		for(auto &merchant : merchantList)
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

std::string InstructionsAnalyser::doUpdateServiceType(web::json::object &object)
{
	string account = object["account"].as_string();
	int maxDistance = object["max_distance"].as_integer();
	web::json::array supportAppliances = object["support_appliance"].as_array();
	list<string> supportAppliancesList;
	for(auto &s : supportAppliances)
		supportAppliancesList.push_back(s.as_string());

	AccountManager::getInstance().updateSupportedServiceFor(account, supportAppliancesList, maxDistance);

	return "";
}