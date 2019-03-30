//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_INSTRUCTIONSANALYSER_H
#define HARSERVER_INSTRUCTIONSANALYSER_H

#include <map>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>

class MerchantAccount;
class Account;

class InstructionsAnalyser {
public:
	std::string instructionFromMap(std::map<utility::string_t, utility::string_t> instruction);
	std::string instructionFromJson(web::json::value json);

private:
	// about login
	std::string doGetVerification(std::map<utility::string_t, utility::string_t> &instruction);
	std::string doLogin(web::json::object &object);

	std::string doUpdateServiceType(web::json::object &object);

	// about order
	std::string doSubmitOrder(web::json::object &object);
	std::string doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction);
	std::string doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction);

	// about get lists
	std::string doGetList(std::map<utility::string_t, utility::string_t> &instruction);
	std::string getServiceTypeList(std::weak_ptr<MerchantAccount> account);
	std::string getMerchantList();
	std::string getApplianceTypeList();
	std::string getOrderList(std::weak_ptr<Account> account);
	std::string getUnreceivedOrderForCustomer(std::weak_ptr<MerchantAccount> account);

	unsigned long toUnsignedLong(std::string s);
	double toDouble(std::string s);
};


#endif //HARSERVER_INSTRUCTIONSANALYSER_H
