//
// Created by gaojian on 19-3-19.
//

#include "InstructionsAnalyser.h"
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include "Account/MerchantServiceType.h"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "Account/ContactInformation.h"
#include "Errors/DatabaseInternalError.hpp"
#include "Errors/NoSuchAnAccountError.hpp"
#include "Errors/PasswordNotRightError.hpp"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"
#include <ctime>

using std::string;							using std::cerr;
using std::endl;							using std::list;
using std::weak_ptr;						using std::istringstream;
using std::cout;							using utility::conversions::to_utf8string;
using utility::conversions::to_string_t;	using std::make_shared;
using std::ctime;

InstructionsAnalyser::InstructionsAnalyser()
{
	orderManagerProxy = make_shared<OrderManagerProxy>();
	accountManagerProxy = make_shared<AccountManagerProxy>();
}

utility::string_t InstructionsAnalyser::instructionFromMap(std::map<utility::string_t, utility::string_t> instruction)
{
	utility::string_t replyMsg;

	if(instruction[U("type")] == U("get_verification"))				// get verification
		replyMsg = doGetVerification(instruction);
	else if(instruction.find(U("get_list")) != instruction.end())	// get merchant/appliance type list
		replyMsg = doGetList(instruction);
	else if(instruction[U("type")] == U("update_order"))
		replyMsg = doUpdateOrderState(instruction);
	else if(instruction[U("type")] == U("get_order_detail"))
		replyMsg = doGetOrderDetail(instruction);
	else
		std::cerr << "No such an instruction" << std::endl;

	return replyMsg;
}

utility::string_t InstructionsAnalyser::instructionFromJson(web::json::value json)
{
	utility::string_t replyMsg;

	web::json::object object = json.as_object();
	string type = to_utf8string(object.at(U("type")).as_string());
	if(type == "login")					// login
		replyMsg = doLogin(object);
	else if(type == "submit_service_type")
		replyMsg = doUpdateServiceType(object);
	else if(type == "submit_order")
		replyMsg = doSubmitOrder(object);

	return replyMsg;
}

utility::string_t InstructionsAnalyser::doGetVerification(std::map<utility::string_t, utility::string_t> &instruction)
{
	try
	{
		if (instruction[U("account_type")] == U("merchant"))
			accountManagerProxy->merchantRequestForVerificationCode(to_utf8string(instruction[U("account")]));
		else if (instruction[U("account_type")] == U("customer"))
			accountManagerProxy->customerRequestForVerificationCode(to_utf8string(instruction[U("account")]));
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return U("");
}

utility::string_t InstructionsAnalyser::doLogin(web::json::object &object)
{
	web::json::value retJson;

	try
	{
		string account = to_utf8string(object.at(U("account")).as_string());
		string password = to_utf8string(object.at(U("password")).as_string());
		string accountType = to_utf8string(object.at(U("account_type")).as_string());
		if(accountType == "merchant")
		{
			auto merchant = accountManagerProxy->merchantAuthentication(account, password);
			// orderManagerProxy->loadOrderForAccount(merchant);
		}
		else if(accountType == "customer")
		{
			auto customer = accountManagerProxy->customerAuthentication(account, password);
			// orderManagerProxy->loadOrderForAccount(customer);
		}
		retJson[U("login_result")] = web::json::value(U("success"));
	}
	catch (NoSuchAnAccountError &e)
	{
		retJson[U("login_result")] = web::json::value(U("no such an account"));
	}
	catch (PasswordNotRightError &e)
	{
		retJson[U("login_result")] = web::json::value(U("wrong password"));
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::doGetList(std::map<utility::string_t, utility::string_t> &instruction)
{
	utility::string_t ret;

	string getListType = to_utf8string(instruction[U("get_list")]);
	if(getListType == "origin_service_type")	// get origin service types
	{
		weak_ptr<MerchantAccount> account = accountManagerProxy->getMerchant(to_utf8string(instruction[U("account"])));
		ret = getServiceTypeList(account);
	}
	else if(getListType == "merchant_list")		// get merchant list
		ret = getMerchantList();
	else if(getListType == "appliance_type")	// get appliance type list
		ret = getApplianceTypeList();
	else if(getListType == "order_list")		// get order list
	{
		weak_ptr<Account> account;
		if(instruction[U("account_type")] == U("customer"))
			account = accountManagerProxy->getCustomer(to_utf8string(instruction[U("account"])));
		else
			account = accountManagerProxy->getMerchant(to_utf8string(instruction[U("account"])));
		if(account.lock())
			ret = getOrderList(account);
	}
	else if(getListType == "unreceived_order")	// get unreceived order list
	{
		weak_ptr<MerchantAccount> account = accountManagerProxy->getMerchant(to_utf8string(instruction[U("account"])));
		ret = getUnreceivedOrderForCustomer(account);
	}

	return ret;
}

utility::string_t InstructionsAnalyser::getServiceTypeList(std::weak_ptr<MerchantAccount> &account)
{
	web::json::value retJson;

	if(account.lock() != nullptr)
	{
		weak_ptr<MerchantServiceType> serviceType = account.lock()->supportedServiceType();
		web::json::value array;
		list<string> supportList = serviceType.lock()->supportApplianceType();
		auto it = supportList.begin();
		for (unsigned i = 0; it != supportList.end(); ++i, ++it)
			array[i] = web::json::value(to_string_t(*it));
		retJson[U("support_appliance_type")] = array;
		retJson[U("max_distance")] = web::json::value(serviceType.lock()->maxRepairDistance());
	}
	else
		retJson[U("result")] = web::json::value(U("empty"));

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::getMerchantList()
{
	web::json::value retJson;

	list<weak_ptr<MerchantAccount>> merchants = accountManagerProxy->getMerchantList();
	web::json::value array;
	auto it = merchants.begin();
	for(unsigned i = 0; it != merchants.end(); ++i, ++it)
		array[i] = web::json::value(to_string_t(it->lock()->account()));
	retJson[U("merchant_list")] = array;

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::getApplianceTypeList()
{
	web::json::value retJson;

	list<weak_ptr<MerchantAccount>> merchantList = accountManagerProxy->getMerchantList();
	web::json::value array;
	for(auto &merchant : merchantList)
	{
		list<string> typeList = merchant.lock()->supportedServiceType().lock()->supportApplianceType();
		auto it = typeList.begin();
		for(unsigned i = 0; it != typeList.end(); ++i, ++it)
			array[i] = web::json::value(to_string_t(*it));
	}
	retJson[U("appliance_list")] = array;

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::doUpdateServiceType(web::json::object &object)
{
	web::json::value retJson;

	string account = to_utf8string(object[U("account")].as_string());
	int maxDistance = object[U("max_distance")].as_integer();
	web::json::array supportAppliances = object[U("support_appliance")].as_array();
	list<string> supportAppliancesList;
	for(auto &s : supportAppliances)
		supportAppliancesList.push_back(to_utf8string(s.as_string()));

	auto merchant = accountManagerProxy->getMerchant(account);
	if(merchant.lock() != nullptr)
		merchant.lock()->updateSupportedService(supportAppliancesList, maxDistance);
	else
		retJson[U("result")] = web::json::value(U("empty"));

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::getOrderList(std::weak_ptr<Account> &account)
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
		orderDetail[U("create_date")] = web::json::value(to_string_t(date));
		orderDetail[U("appliance_type")] = web::json::value(to_string_t(type));
		orderDetail[U("id")] = web::json::value::number(static_cast<uint64_t>(id));
		array[i] = orderDetail;
	}
	retJson[U("order_list")] = array;

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::getUnreceivedOrderForCustomer(std::weak_ptr<MerchantAccount> &account)
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
		orderDetail[U("create_date")] = web::json::value(to_string_t(date));
		orderDetail[U("appliance_type")] = web::json::value(to_string_t(type));
		orderDetail[U("detail")] = web::json::value(to_string_t(detail));
		array[i] = orderDetail;
	}
	retJson[U("order_list")] = array;

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::doSubmitOrder(web::json::object &object)
{
	web::json::value retJson;

	try
	{
		string appliance = to_utf8string(object[U("appliance")].as_string());
		string merchantAccount = to_utf8string(object[U("merchant")].as_string());
		string customerAccount = to_utf8string(object[U("account")].as_string());
		string detailDescription = to_utf8string(object[U("detail")].as_string());
		string address = to_utf8string(object[U("address")].as_string());

		auto customer = accountManagerProxy->getCustomer(customerAccount);
		auto merchant = accountManagerProxy->getMerchant(merchantAccount);
		if (customer.lock() != nullptr && merchant.lock() != nullptr)
		{
			ContactInformation tmp(address, "");
			AcceptableOrderPriceRange range;
			orderManagerProxy->publishOrder(customer, merchant,
													 appliance, tmp, detailDescription,
													 range
			);
		} else
			retJson[U("result")] = web::json::value(U("empty"));
	}
	catch (DatabaseInternalError &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction)
{
	utility::string_t ret;

	weak_ptr<MerchantAccount> merchant = accountManagerProxy->getMerchant(to_utf8string(instruction[U("account"])));
	unsigned long id = toUnsignedLong(to_utf8string(instruction[U("order_id")]));
	weak_ptr<Order> order = orderManagerProxy->getOrder(id);

	if(instruction[U("order_operate")] == U("accept"))
		orderManagerProxy->orderAccepted(merchant, order);
	else if(instruction[U("order_operate")] == U("reject"))
		orderManagerProxy->orderRejected(merchant, order);
	else if(instruction[U("order_operate")] == U("start_repair"))
		orderManagerProxy->orderStartRepair(merchant, order);
	else if(instruction[U("order_operate")] == U("end_repair"))
	{
		double transaction = toDouble(to_utf8string(instruction[U("transaction")]));
		orderManagerProxy->orderEndRepair(merchant, order, transaction);
	}

	return ret;
}

utility::string_t InstructionsAnalyser::doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction)
{
	web::json::value retJson;

	unsigned long id = toUnsignedLong(to_utf8string(instruction[U("order_id")]));
	auto order = orderManagerProxy->getOrder(id);

	try
	{
		if (order.lock() != nullptr)
		{
			retJson[U("id")] = web::json::value(static_cast<uint64_t>(order.lock()->id()));
			retJson[U("appliance_type")] = web::json::value(to_string_t(order.lock()->applianceType()));
			retJson[U("detail")] = web::json::value(to_string_t(order.lock()->detail()));
			OrderState::States currentState = order.lock()->currentState();
			retJson[U("state")] = web::json::value(to_string_t(getOrderStateString(currentState)));

			std::time_t time = std::chrono::system_clock::to_time_t(order.lock()->createDate());
			retJson[U("create_date")] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->receiveDate());
			retJson[U("received_date")] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->startRepairDate());
			retJson[U("start_repair_date")] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->endRepairDate());
			retJson[U("end_repair_date")] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->finishDate());
			retJson[U("finish_date")] = web::json::value(ctime(&time));
			time = std::chrono::system_clock::to_time_t(order.lock()->rejectDate());
			retJson[U("reject_date")] = web::json::value(ctime(&time));
		}
	}
	catch (OrderNotAtRightState &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

utility::string_t InstructionsAnalyser::getOrderStateString(OrderState::States &state)
{
	utility::string_t stateString;
	switch (state)
	{
		case OrderState::States::unreceivedState:
			stateString = U("unreceived");
			break;
		case OrderState::States::receivedState:
			stateString = U("received");
			break;
		case OrderState::States::startRepairState:
			stateString = U("repairing");
			break;
		case OrderState::States::endRepairState:
			stateString = U("paying");
			break;
		case OrderState::States::finishedState:
			stateString = U("finished");
			break;
		case OrderState::States::rejectState:
			stateString = U("reject");
			break;
	}
	return stateString;
}

unsigned long InstructionsAnalyser::toUnsignedLong(const std::string &s)
{
	istringstream stream(s);
	unsigned long num;
	stream >> num;
	return num;
}

double InstructionsAnalyser::toDouble(const std::string &s)
{
	istringstream stream(s);
	double num;
	stream >> num;
	return num;
}