//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_INSTRUCTIONSANALYSER_H
#define HARSERVER_INSTRUCTIONSANALYSER_H

#include <map>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include "Order/OrderStates/OrderState.h"

class MerchantAccount;
class Account;
class OrderManagerProxy;
class AccountManagerProxy;

class InstructionsAnalyser {
public:
	InstructionsAnalyser();

	utility::string_t instructionFromMap(std::map<utility::string_t, utility::string_t> instruction);
	utility::string_t instructionFromJson(web::json::value json);

private:
	// about login
	utility::string_t doGetVerification(std::map<utility::string_t, utility::string_t> &instruction);
	utility::string_t doLogin(web::json::object &object);

	utility::string_t doUpdateServiceType(web::json::object &object);

	// about order
	utility::string_t doSubmitOrder(web::json::object &object);
	utility::string_t doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction);
	utility::string_t doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction);

	// about get lists
	utility::string_t doGetList(std::map<utility::string_t, utility::string_t> &instruction);
	utility::string_t getServiceTypeList(std::weak_ptr<MerchantAccount> &account);
	utility::string_t getMerchantList();
	utility::string_t getApplianceTypeList();
	utility::string_t getOrderList(std::weak_ptr<Account> &account);
	utility::string_t getUnreceivedOrderForMerchant(std::weak_ptr<MerchantAccount> &account);

	utility::string_t getOrderStateString(OrderState::States &&state);
	unsigned long toUnsignedLong(const std::string &s);
	double toDouble(const std::string &s);

	std::shared_ptr<OrderManagerProxy> orderManagerProxy;
	std::shared_ptr<AccountManagerProxy> accountManagerProxy;
};


#endif //HARSERVER_INSTRUCTIONSANALYSER_H
