//
// Created by gaojian on 19-3-19.
//

#include "RequestInterpreter.h"
#include <ctime>
#include <Account/Account.h>
#include "RespondGenerator.h"
#include "RestHandler.h"

using std::string;							using std::cerr;
using std::endl;							using std::list;
using std::weak_ptr;						using std::istringstream;
using std::cout;							using utility::conversions::to_utf8string;
using utility::conversions::to_string_t;	using std::make_unique;
using std::ctime;

RequestInterpreter::RequestInterpreter()
{
	respondGenerator = make_unique<RespondGenerator>();
}

Respond RequestInterpreter::analysis(HTTPRequest request, std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object data)
{
	Respond replyMsg;

	if(uri[0] == U("verification"))
		replyMsg = requestVerification(uri);
	else if(uri[0] == U("token"))
		replyMsg = getToken(uri, data);
	else if(uri[0] == U("merchant_list"))
		replyMsg = getMerchantsList(uri);
	else if(request == HTTPRequest::POST && uri[0] == U("order"))
		replyMsg = publishOrder(data);
	else if(uri[0] == U("allowed_service"))
		replyMsg = getAllowedService();
	else if(uri[0] == U("supported_service"))
		replyMsg = getMerchantService(uri);
	else if(request == HTTPRequest::PUT && uri[0] == U("supported_service"))
		replyMsg = updateMerchantService(uri, headers, data);
	else if(uri[0] == U("order_list"))
		replyMsg = getOrderList(uri, headers);
	else if(request == HTTPRequest::GET && uri[0] == U("order"))
		replyMsg = getOrderDetail(uri, headers);
	else if(request == HTTPRequest::PUT && uri[0] == U("order"))
		replyMsg = updateOrder(uri, headers, data);
	/*if(instruction[U("type")] == U("get_verification"))				// get verification
		replyMsg = requestVerification(instruction);
	else if(instruction.find(U("get_list")) != instruction.end())	// get merchant/appliance type list
		replyMsg = doGetList(instruction);
	else if(instruction[U("type")] == U("update_order"))
		replyMsg = doUpdateOrderState(instruction);
	else if(instruction[U("type")] == U("get_order_detail"))
		replyMsg = doGetOrderDetail(instruction);
	else
		std::cerr << "No such an instruction" << std::endl;*/

	return std::move(replyMsg);
}

/*utility::string_t RequestInterpreter::analysis(web::json::value json)
{
	utility::string_t replyMsg;

	web::json::object object = json.as_object();
	string type = to_utf8string(object.at(U("type")).as_string());
	if(type == "login")					// login
		replyMsg = getToken(object);
	else if(type == "submit_service_type")
		replyMsg = doUpdateServiceType(object);
	else if(type == "submit_order")
		replyMsg = doSubmitOrder(object);

	return replyMsg;
}*/

Respond RequestInterpreter::requestVerification(std::vector<utility::string_t> uri)
{
	Account::Type accountType = uri[1] == U("merchant") ? Account::Type::Merchant : Account::Type::Customer;
	std::string account = to_utf8string(uri[2]);

	return respondGenerator->requestVerification(accountType, account);
}

Respond RequestInterpreter::getToken(std::vector<utility::string_t> uri, web::json::object json)
{
	Account::Type accountType = uri[1] == U("merchant") ? Account::Type::Merchant : Account::Type::Customer;
	std::string account = to_utf8string(uri[2]);
	std::string password = to_utf8string(json[0].as_string());

	return respondGenerator->getToken(accountType, account, password);
}

Respond RequestInterpreter::getMerchantsList(std::vector<utility::string_t> uri)
{
	double longitude = toDouble(uri[1]);
	double latitude = toDouble(uri[1]);

	return respondGenerator->getMerchantsList(longitude, latitude);
}

Respond RequestInterpreter::publishOrder(web::json::object json)
{
	unsigned long receiver = json[U("receiver")].as_integer();
	unsigned long committer = json[U("committer")].as_integer();
	string applianceType = json[U("appliance")].as_string();
	string details = json[U("detail")].as_string();

	return respondGenerator->publishOrder(receiver, committer, applianceType, details);
}

Respond RequestInterpreter::getAllowedService()
{
	return respondGenerator->getAllowedService();
}

Respond RequestInterpreter::getMerchantService(std::vector<utility::string_t> uri)
{
	unsigned long merchantId = toUnsignedLong(uri[1]);
	return respondGenerator->getMerchantService(merchantId);
}

Respond RequestInterpreter::updateMerchantService(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long merchantId = toUnsignedLong(uri[1]);
	double distance = json[U("max_distance")].as_double();
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	std::list<std::string> appliances;
	web::json::array applianceArray = json[U("support_appliance")].as_array();
	for (auto &a : applianceArray)
		appliances.emplace_back(a.as_string());

	return respondGenerator->updateMerchantService(tokenId, merchantId, distance, appliances);
}

Respond RequestInterpreter::getOrderList(std::vector<utility::string_t> uri, web::http::http_headers headers)
{
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	Account::Type accountType = uri[1] == U("merchant") ? Account::Type::Merchant : Account::Type::Customer;
	unsigned long merchantId = toUnsignedLong(uri[2]);

	return respondGenerator->getOrderList(tokenId, accountType, merchantId);
}

Respond RequestInterpreter::getOrderDetail(std::vector<utility::string_t> uri, web::http::http_headers headers)
{
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long orderId = toUnsignedLong(uri[1]);

	return respondGenerator->getOrderDetail(tokenId, orderId);
}

Respond RequestInterpreter::updateOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	string type = json[U("state")].as_string();
	if(type == "received")
		return receiveOrder(uri, headers, json);
	else if(type == "start_repair")
		return startRepair(uri, headers, json);
	else if(type == "end_repair")
		return endRepair(uri, headers, json);
	else if(type == "pay")
		return pay(uri, headers, json);
	else if(type == "reject")
		return rejectOrder(uri, headers, json);
	else
		return std::move(Respond());
}

Respond RequestInterpreter::receiveOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long orderId = toUnsignedLong(uri[1]);
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long accountId = json[U("account")].as_integer();
	return respondGenerator->receiveOrder(tokenId, orderId, accountId);
}

Respond RequestInterpreter::startRepair(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long orderId = toUnsignedLong(uri[1]);
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long accountId = json[U("account")].as_integer();
	return respondGenerator->startRepair(tokenId, orderId, accountId);
}

Respond RequestInterpreter::endRepair(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long orderId = toUnsignedLong(uri[1]);
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long accountId = json[U("account")].as_integer();
	double transaction = json[U("transaction")].as_double();
	return respondGenerator->endRepair(tokenId, orderId, accountId, transaction);
}

Respond RequestInterpreter::pay(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long orderId = toUnsignedLong(uri[1]);
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long accountId = json[U("account")].as_integer();
	return respondGenerator->pay(tokenId, orderId, accountId);
}

Respond RequestInterpreter::rejectOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json)
{
	unsigned long orderId = toUnsignedLong(uri[1]);
	unsigned long tokenId = toUnsignedLong(headers[U("token")]);
	unsigned long accountId = json[U("account")].as_integer();
	return respondGenerator->rejectOrder(tokenId, orderId, accountId);
}

/*
utility::string_t RequestInterpreter::doGetList(std::map<utility::string_t, utility::string_t> &instruction)
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
		ret = getUnreceivedOrderForMerchant(account);
	}

	return ret;
}

utility::string_t RequestInterpreter::getServiceTypeList(std::weak_ptr<MerchantAccount> &account)
{
	web::json::value retJson;

	if(account.lock() != nullptr)
	{
		MerchantServiceType *serviceType = account.lock()->supportedServiceType();
		web::json::value array;
		list<string> supportList = serviceType->supportApplianceType();
		auto it = supportList.begin();
		for (unsigned i = 0; it != supportList.end(); ++i, ++it)
			array[i] = web::json::value(to_string_t(*it));
		retJson[U("support_appliance_type")] = array;
		retJson[U("max_distance")] = web::json::value(serviceType->maxRepairDistance());
	}
	else
		retJson[U("result")] = web::json::value(U("empty"));

	return retJson.serialize();
}

utility::string_t RequestInterpreter::getMerchantList()
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

utility::string_t RequestInterpreter::getApplianceTypeList()
{
	web::json::value retJson;

	list<weak_ptr<MerchantAccount>> merchantList = accountManagerProxy->getMerchantList();
	web::json::value array;
	for(auto &merchant : merchantList)
	{
		list<string> typeList = merchant.lock()->supportedServiceType()->supportApplianceType();
		auto it = typeList.begin();
		for(unsigned i = 0; it != typeList.end(); ++i, ++it)
			array[i] = web::json::value(to_string_t(*it));
	}
	retJson[U("appliance_list")] = array;

	return retJson.serialize();
}

utility::string_t RequestInterpreter::doUpdateServiceType(web::json::object &object)
{
	web::json::value retJson;

	string account = to_utf8string(object[U("account")].as_string());
	double maxDistance = object[U("max_distance")].as_double();
	web::json::array supportAppliances = object[U("support_appliance")].as_array();
	list<string> supportAppliancesList;
	for(auto &s : supportAppliances)
		supportAppliancesList.push_back(to_utf8string(s.as_string()));

	accountManagerProxy->updateServiceTypeFor(account, supportAppliancesList, maxDistance);

	return retJson.serialize();
}

utility::string_t RequestInterpreter::getOrderList(std::weak_ptr<Account> &account)
{
	web::json::value retJson;

	web::json::value array;
	auto list = account.lock()->myOrdersList();
	auto it = list.begin();
	for(unsigned i = 0; it != list.end(); ++it, ++i)
	{
		web::json::value orderDetail;
		std::time_t t = std::chrono::system_clock::to_time_t((*it).lock()->createDate());
		orderDetail[U("create_date")] = web::json::value(to_string_t(ctime(&t)));
		orderDetail[U("appliance_type")] = web::json::value(to_string_t((*it).lock()->applianceType()));
		orderDetail[U("id")] = web::json::value::number(static_cast<uint64_t>((*it).lock()->id()));
		orderDetail[U("current_state")] = web::json::value(to_string_t(getOrderStateString((*it).lock()->currentState())));
		array[i] = orderDetail;
	}
	retJson[U("order_list")] = array;

	return retJson.serialize();
}

utility::string_t RequestInterpreter::getUnreceivedOrderForMerchant(std::weak_ptr<MerchantAccount> &account)
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

utility::string_t RequestInterpreter::doSubmitOrder(web::json::object &object)
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
			ContactInformation tmp;
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

utility::string_t RequestInterpreter::doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction)
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

utility::string_t RequestInterpreter::doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction)
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
			retJson[U("state")] = web::json::value(to_string_t(getOrderStateString(order.lock()->currentState())));

			std::time_t time = std::chrono::system_clock::to_time_t(order.lock()->createDate());
			retJson[U("create_date")] = web::json::value(to_string_t(ctime(&time)));
			time = std::chrono::system_clock::to_time_t(order.lock()->receiveDate());
			retJson[U("received_date")] = web::json::value(to_string_t(ctime(&time)));
			time = std::chrono::system_clock::to_time_t(order.lock()->startRepairDate());
			retJson[U("start_repair_date")] = web::json::value(to_string_t(ctime(&time)));
			time = std::chrono::system_clock::to_time_t(order.lock()->endRepairDate());
			retJson[U("end_repair_date")] = web::json::value(to_string_t(ctime(&time)));
			time = std::chrono::system_clock::to_time_t(order.lock()->finishDate());
			retJson[U("finish_date")] = web::json::value(to_string_t(ctime(&time)));
			time = std::chrono::system_clock::to_time_t(order.lock()->rejectDate());
			retJson[U("reject_date")] = web::json::value(to_string_t(ctime(&time)));
		}
	}
	catch (OrderNotAtRightState &e)
	{
		cerr << e.what() << endl;
	}

	return retJson.serialize();
}

utility::string_t RequestInterpreter::getOrderStateString(OrderState::States &&state)
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



double RequestInterpreter::toDouble(const std::string &s)
{
	istringstream stream(s);
	double num;
	stream >> num;
	return num;
}*/

unsigned long RequestInterpreter::toUnsignedLong(const std::string &s)
{
	istringstream stream(s);
	unsigned long num;
	stream >> num;
	return num;
}

double RequestInterpreter::toDouble(const std::string &s)
{
	istringstream stream(s);
	double num;
	stream >> num;
	return num;
}