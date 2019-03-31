//
// Created by gaojian on 19-3-19.
//

#include "InstructionsAnalyser.h"
#include "AccountManager.h"
#include "OrderManager.h"
#include "../Account/MerchantServiceType.h"
#include "../Account/MerchantAccount.h"
#include "../Account/CustomerAccount.h"
#include "../Account/ContactInformation.h"
#include "../Errors/DatabaseInternalError.h"
#include "../Errors/NoSuchAnAccountError.h"
#include "../Errors/PasswordNotRightError.h"
#include "../Order/OrderStates/AcceptableOrderPriceRange.h"
#include "../Order/Order.h"
#include "../Errors/OrderNotAtRightState.h"

using std::string;					using std::cerr;
using std::endl;					using std::list;
using std::weak_ptr;				using std::istringstream;

std::string InstructionsAnalyser::instructionFromMap(std::map<utility::string_t, utility::string_t> instruction)
{
	string replyMsg;

	if(instruction["type"] == "get_verification")				// get verification
		replyMsg = doGetVerification(instruction);
	else if(instruction.find("get_list") != instruction.end())	// get merchant/appliance type list
		replyMsg = doGetList(instruction);
	else if(instruction["type"] == "update_order")
		replyMsg = doUpdateOrderState(instruction);
	else if(instruction["type"] == "get_order_detail")
		replyMsg = doGetOrderDetail(instruction);
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
	else if(type == "submit_order")
		replyMsg = doSubmitOrder(object);

	return replyMsg;
}

std::string InstructionsAnalyser::doGetVerification(std::map<utility::string_t, utility::string_t> &instruction)
{
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

	return "";
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
	string ret;

	string getListType = instruction["get_list"];
	if(getListType == "origin_service_type")	// get origin service types
	{
		weak_ptr<MerchantAccount> account = AccountManager::getInstance().getMerchant(instruction["account"]);
		ret = getServiceTypeList(account);
	}
	else if(getListType == "merchant_list")		// get merchant list
		ret = getMerchantList();
	else if(getListType == "appliance_type")	// get appliance type list
		ret = getApplianceTypeList();
	else if(getListType == "order_list")		// get order list
	{
		weak_ptr<Account> account;
		if(instruction["account_type"] == "customer")
			account = AccountManager::getInstance().getCustomer(instruction["account"]);
		else
			account = AccountManager::getInstance().getMerchant(instruction["account"]);
		if(account.lock())
			ret = getOrderList(account);
	}
	else if(getListType == "unreceived_order")	// get unreceived order list
	{
		weak_ptr<MerchantAccount> account = AccountManager::getInstance().getMerchant(instruction["account"]);
		ret = getUnreceivedOrderForCustomer(account);
	}

	return ret;
}

std::string InstructionsAnalyser::getServiceTypeList(std::weak_ptr<MerchantAccount> account)
{
	web::json::value retJson;

	if(account.lock() != nullptr)
	{
		weak_ptr<MerchantServiceType> serviceType = account.lock()->supportedServiceType();
		web::json::value array;
		list<string> supportList = serviceType.lock()->supportApplianceType();
		auto it = supportList.begin();
		for (unsigned i = 0; it != supportList.end(); ++i, ++it)
			array[i] = web::json::value(*it);
		retJson["support_appliance_type"] = array;
		retJson["max_distance"] = web::json::value(serviceType.lock()->maxRepairDistance());
	}
	else
		retJson["result"] = web::json::value("empty");

	return retJson.serialize();
}

std::string InstructionsAnalyser::getMerchantList()
{
	web::json::value retJson;

	list<weak_ptr<MerchantAccount>> merchants = AccountManager::getInstance().getMerchantList();
	web::json::value array;
	auto it = merchants.begin();
	for(unsigned i = 0; it != merchants.end(); ++i, ++it)
		array[i] = web::json::value(it->lock()->account());
	retJson["merchant_list"] = array;

	return retJson.serialize();
}

std::string InstructionsAnalyser::getApplianceTypeList()
{
	web::json::value retJson;

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

	return retJson.serialize();
}

std::string InstructionsAnalyser::doUpdateServiceType(web::json::object &object)
{
	web::json::value retJson;

	string account = object["account"].as_string();
	int maxDistance = object["max_distance"].as_integer();
	web::json::array supportAppliances = object["support_appliance"].as_array();
	list<string> supportAppliancesList;
	for(auto &s : supportAppliances)
		supportAppliancesList.push_back(s.as_string());

	auto merchant = AccountManager::getInstance().getMerchant(account);
	if(merchant.lock() != nullptr)
		merchant.lock()->updateSupportedService(supportAppliancesList, maxDistance);
	else
		retJson["result"] = web::json::value("empty");

	return retJson.serialize();
}

std::string InstructionsAnalyser::getOrderList(std::weak_ptr<Account> account)
{
	web::json::value retJson;

	web::json::value array;
	auto list = account.lock()->myOrdersList();
	auto it = list.begin();
	for(unsigned i = 0; it != list.end(); ++it, ++i)
	{
		web::json::value orderDetail;
		std::time_t t = std::chrono::system_clock::to_time_t((*it).lock()->createDate());
		string date = ctime(&t);
		string type = (*it).lock()->applianceType();
		string detail = (*it).lock()->detail();
		unsigned long id = (*it).lock()->id();
		orderDetail["create_date"] = web::json::value(date);
		orderDetail["appliance_type"] = web::json::value(type);
		orderDetail["id"] = web::json::value(id);
		array[i] = orderDetail;
	}
	retJson["order_list"] = array;

	return retJson.serialize();
}

std::string InstructionsAnalyser::getUnreceivedOrderForCustomer(std::weak_ptr<MerchantAccount> account)
{
	web::json::value retJson;

	web::json::value array;
	auto list = account.lock()->myUnreceivedOrderList();
	auto it = list.begin();
	for(unsigned i = 0; it != list.end(); ++it, ++i)
	{
		web::json::value orderDetail;
		std::time_t t = std::chrono::system_clock::to_time_t((*it).lock()->createDate());
		string date = ctime(&t);
		string type = (*it).lock()->applianceType();
		string detail = (*it).lock()->detail();
		orderDetail["create_date"] = web::json::value(date);
		orderDetail["appliance_type"] = web::json::value(type);
		orderDetail["detail"] = web::json::value(detail);
		array[i] = orderDetail;
	}
	retJson["order_list"] = array;

	return retJson.serialize();
}

std::string InstructionsAnalyser::doSubmitOrder(web::json::object &object)
{
	web::json::value retJson;

	try
	{
		string appliance = object["appliance"].as_string();
		string merchantAccount = object["merchant"].as_string();
		string customerAccount = object["account"].as_string();
		string detailDescription = object["detail"].as_string();
		string address = object["address"].as_string();

		auto customer = AccountManager::getInstance().getCustomer(customerAccount);
		auto merchant = AccountManager::getInstance().getMerchant(merchantAccount);
		if (customer.lock() != nullptr && merchant.lock() != nullptr)
		{
			OrderManager::getInstance().publishOrder(customer, merchant,
													 appliance, ContactInformation(address), detailDescription,
													 AcceptableOrderPriceRange()
			);
		} else
			retJson["result"] = web::json::value("empty");
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

std::string InstructionsAnalyser::doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction)
{
	string ret;

	weak_ptr<MerchantAccount> merchant = AccountManager::getInstance().getMerchant(instruction["account"]);
	unsigned long id = toUnsignedLong(instruction["order_id"]);
	weak_ptr<Order> order = OrderManager::getInstance().getOrder(id);

	if(instruction["order_operate"] == "accept")
		OrderManager::getInstance().orderAccepted(merchant, order);
	else if(instruction["order_operate"] == "reject")
		OrderManager::getInstance().orderRejected(merchant, order);
	else if(instruction["order_operate"] == "start_repair")
		OrderManager::getInstance().orderStartRepair(merchant, order);
	else if(instruction["order_operate"] == "end_repair")
	{
		double transaction = toDouble(instruction["transaction"]);
		OrderManager::getInstance().orderEndRepair(merchant, order, transaction);
	}

	return ret;
}

std::string InstructionsAnalyser::doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction)
{
	web::json::value retJson;

	unsigned long id = toUnsignedLong(instruction["order_id"]);
	auto order = OrderManager::getInstance().getOrder(id);

	try
	{
		if (order.lock() != nullptr)
		{
			retJson["id"] = web::json::value(order.lock()->id());
			retJson["appliance_type"] = web::json::value(order.lock()->applianceType());
			retJson["detail"] = web::json::value(order.lock()->detail());
			OrderState::States currentState = order.lock()->currentState();
			retJson["state"] = web::json::value(getOrderStateString(currentState));

			std::time_t time = std::chrono::system_clock::to_time_t(order.lock()->createDate());
			retJson["create_date"] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->receiveDate());
			retJson["received_date"] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->startRepairDate());
			retJson["start_repair_date"] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->endRepairDate());
			retJson["end_repair_date"] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->finishDate());
			retJson["finish_date"] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->rejectDate());
			retJson["reject_date"] = web::json::value(ctime(&time));
		}
	}
	catch (OrderNotAtRightState &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

std::string InstructionsAnalyser::getOrderStateString(OrderState::States &state)
{
	string stateString;
	switch (state)
	{
		case OrderState::States::unreceivedState:
			stateString = "unreceived";
			break;
		case OrderState::States::receivedState:
			stateString = "received";
			break;
		case OrderState::States::startRepairState:
			stateString = "repairing";
			break;
		case OrderState::States::endRepairState:
			stateString = "paying";
			break;
		case OrderState::States::finishedState:
			stateString = "finished";
			break;
		case OrderState::States::rejectState:
			stateString = "reject";
			break;
	}
	return stateString;
}

unsigned long InstructionsAnalyser::toUnsignedLong(std::string s)
{
	istringstream istr(s);
	unsigned long num;
	istr >> num;
	return num;
}

double InstructionsAnalyser::toDouble(std::string s)
{
	istringstream istr(s);
	double num;
	istr >> num;
	return num;
}