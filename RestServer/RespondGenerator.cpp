//
// Created by 17271 on 2019/9/23.
//

#include "RespondGenerator.h"
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include "ManagerProxy/TokenManagerProxy.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h"
#include "RestHandler.h"
#include "Account/MerchantAccount.h"
#include "Account/MerchantService.h"
#include "Order/Order.h"
#include <string>

using std::make_unique;				using std::to_string;
using std::shared_ptr;				using std::weak_ptr;
using std::list;

RespondGenerator::RespondGenerator()
{
	orderManagerProxy = make_unique<OrderManagerProxy>();
	accountManagerProxy = make_unique<AccountManagerProxy>();
	tokenManagerProxy = make_unique<TokenManagerProxy>();
	authenticationCarrier = make_unique<AuthenticationToScreen>();
}

Respond RespondGenerator::requestVerification(Account::Type accountType, std::string account)
{
	unsigned long verification;
	verification = authenticationCarrier->sendVerificationCode(account);
	accountManagerProxy->createOrUpdatePasswordForAccount(accountType, account, to_string(verification));

	return std::move(Respond(web::http::status_codes::OK));
}

Respond RespondGenerator::getToken(Account::Type accountType, std::string account, std::string password)
{
	web::json::value json;
	unsigned long accountId;
	unsigned long tokenId;
	accountId = accountManagerProxy->getAccountIdIfPasswordRight(account, password);
	tokenId = tokenManagerProxy->newToken(accountManagerProxy->loadAccount(accountType, accountId));
	json[U("token")] = tokenId;
	json[U("id")] = accountId;

	return std::move(Respond(web::http::status_codes::OK, std::move(json)));
}

Respond RespondGenerator::getMerchantsList(double longitude, double latitude)
{
	web::json::value json;
	web::json::value merchantArray;

	auto merchants = accountManagerProxy->getMerchantList();
	unsigned long index = 0;
	for (MerchantAccount *m : merchants)
	{
		web::json::value merchant;
		merchant[U("id")] = m->id();
		// TODO: merchant[U("name")] = web::json::value(m->name());
		merchantArray[index++] = merchant;
	}
	json[U("merchants")] = merchantArray;
	return std::move(Respond(web::http::status_codes::OK, std::move(json)));
}

Respond RespondGenerator::publishOrder(unsigned long receiver, unsigned long committer, std::string applianceType,
									 std::string detail)
{
	shared_ptr<Order> newOrder = orderManagerProxy->createOrder(committer, receiver, applianceType, detail);
	CustomerAccount *customer = accountManagerProxy->getCustomer(committer);
	MerchantAccount *merchant = accountManagerProxy->getMerchant(receiver);
	customer->iAmPublishAnOrder(newOrder);
	merchant->orderWaitToBeAccept(newOrder);
	return std::move(Respond());
}

Respond RespondGenerator::getAllowedService()
{
	auto services = MerchantService::getAllowedService();
	web::json::value servicesArray;
	web::json::value ret;
	unsigned long index = 0;
	for (auto &s : services)
		servicesArray[index++] = web::json::value(s);
	ret[U("allowed_services")] = servicesArray;
	return std::move(Respond(std::move(ret)));
}

Respond RespondGenerator::getMerchantService(unsigned long merchant)
{
	MerchantAccount *m = accountManagerProxy->getMerchant(merchant);
	MerchantService *service = m->getService();
	web::json::value ret;
	unsigned long index = 0;
	ret[U("max_distance")] = service->maxRepairDistance();
	web::json::value applianceArray;
	for (auto &s : service->supportApplianceType())
		applianceArray[index++] = web::json::value(s);
	ret[U("supported_appliance")] = applianceArray;

	return std::move(Respond(web::http::status_codes::OK, std::move(ret)));
}

Respond RespondGenerator::updateMerchantService(unsigned long tokenId, unsigned long merchantId, double distance,
												  std::list<std::string> appliances)
{
	web::http::status_code statusCode = web::http::status_codes::NotFound;
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		MerchantService *service = accountManagerProxy->getMerchant(merchantId)->getService();
		service->updateService(appliances, distance);
		statusCode = web::http::status_codes::OK;
	}
	return std::move(Respond(statusCode));
}

Respond RespondGenerator::getOrderList(unsigned long tokenId, Account::Type type, unsigned long accountId)
{
	web::http::status_code statusCode = web::http::status_codes::NotFound;
	web::json::value json;
	web::json::value orderArray;
	unsigned long index = 0;
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		CustomerAccount *account;
		switch(type)
		{
			case Account::Type::Merchant:
				account = accountManagerProxy->getMerchant(accountId);
				break;
			case Account::Type::Customer:
				account = accountManagerProxy->getCustomer(accountId);
				break;
		}
		list<weak_ptr<Order>> orderList = account->getOrderList();
		web::json::value orderItem;
		for (auto &o : orderList)
		{
			std::time_t t = std::chrono::system_clock::to_time_t(o.lock()->createDate());
			orderItem[U("create_date")] = web::json::value(ctime(&t));
			orderItem[U("appliance_type")] = web::json::value(o.lock()->applianceType());
			orderItem[U("id")] = web::json::value(o.lock()->id());
			orderArray[index++] = orderItem;
		}
	}
	json[U("order_list")] = orderArray;

	return std::move(Respond(std::move(statusCode), std::move(json)));
}

Respond RespondGenerator::getOrderDetail(unsigned long tokenId, unsigned long orderId)
{
	web::http::status_code statusCode = web::http::status_codes::NotFound;
	web::json::value json;
	auto timeToStr = [](std::chrono::system_clock::time_point timePoint)
		{ std::time_t t = std::chrono::system_clock::to_time_t(timePoint); return ctime(&t); };
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		json[U("id")] = web::json::value(order.lock()->id());
		json[U("appliance_type")] = web::json::value(order.lock()->applianceType());
		json[U("detail")] = web::json::value(order.lock()->detail());
		switch(order.lock()->currentState())
		{
			case OrderState::States::rejectState:
				json[U("state")] = web::json::value("rejected");
				json[U("reject_date")] = web::json::value(timeToStr(order.lock()->rejectDate()));
				break;
			case OrderState::States::finishedState:
				json[U("state")] = web::json::value("finished");
				json[U("finish_date")] = web::json::value(timeToStr(order.lock()->finishDate()));
			case OrderState::States::endRepairState:
				json[U("state")] = web::json::value("end_repair");
				json[U("end_repair_date")] = web::json::value(timeToStr(order.lock()->endRepairDate()));
			case OrderState::States::startRepairState:
				json[U("state")] = web::json::value("start_repair");
				json[U("start_repair_date")] = web::json::value(timeToStr(order.lock()->startRepairDate()));
			case OrderState::States::receivedState:
				json[U("state")] = web::json::value("received");
				json[U("received_date")] = web::json::value(timeToStr(order.lock()->receiveDate()));
			case OrderState::States::unreceivedState:
				json[U("state")] = web::json::value("unreceived");
				json[U("create_date")] = web::json::value(timeToStr(order.lock()->createDate()));
		}
	}
	return std::move(Respond(std::move(json)));
}

Respond RespondGenerator::receiveOrder(unsigned long tokenId, unsigned long orderId, unsigned long accountId)
{
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		MerchantAccount *account = accountManagerProxy->getMerchant(accountId);
		order.lock()->receive(account);
	}
	return std::move(Respond());
}

Respond RespondGenerator::startRepair(unsigned long tokenId, unsigned long orderId, unsigned long accountId)
{
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		MerchantAccount *account = accountManagerProxy->getMerchant(accountId);
		order.lock()->startRepair(account);
	}
	return std::move(Respond());
}

Respond RespondGenerator::endRepair(unsigned long tokenId, unsigned long orderId, unsigned long accountId, double transaction)
{
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		MerchantAccount *account = accountManagerProxy->getMerchant(accountId);
		order.lock()->endRepair(account, transaction);
	}
	return std::move(Respond());
}

Respond RespondGenerator::pay(unsigned long tokenId, unsigned long orderId, unsigned long accountId)
{
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		CustomerAccount *account = accountManagerProxy->getCustomer(accountId);
		order.lock()->pay(account);
	}
	return std::move(Respond());
}

Respond RespondGenerator::rejectOrder(unsigned long tokenId, unsigned long orderId, unsigned long accountId)
{
	if(tokenManagerProxy->tokenExist(tokenId))
	{
		weak_ptr<Order> order = orderManagerProxy->getOrder(orderId);
		MerchantAccount *account = accountManagerProxy->getMerchant(accountId);
		order.lock()->reject(account);
	}
	return std::move(Respond());
}
