//
// Created by 17271 on 2019/9/23.
//

#ifndef HARSERVER_RESPONDGENERATOR_H
#define HARSERVER_RESPONDGENERATOR_H

#include <vector>
#include <map>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include "Account/Account.h"
/// unique_ptr error !!!
#include "ManagerProxy/Subjects/OrderManagerSubject.h"
#include "ManagerProxy/Subjects/AccountManagerSubject.h"
#include "ManagerProxy/Subjects/TokenManagerSubject.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.h"
/// unique_ptr error !!!

//class OrderManagerSubject;
//class AccountManagerSubject;
//class TokenManagerSubject;
//class AuthenticationCarrier;
class Respond;

class RespondGenerator {
public:
	RespondGenerator();

	// about login
	Respond requestVerification(Account::Type accountType, std::string account);
	Respond getToken(Account::Type accountType, std::string account, std::string password);
	Respond getMerchantsList(double longitude, double latitude);
	Respond publishOrder(unsigned long receiver, unsigned long committer, std::string applianceType, std::string detail);
	Respond getAllowedService();
	Respond getMerchantService(unsigned long merchant);
	Respond updateMerchantService(unsigned long tokenId, unsigned long merchantId, double distance, std::list<std::string> appliances);
	Respond getOrderList(unsigned long tokenId, Account::Type type, unsigned long accountId);
	Respond getOrderDetail(unsigned long tokenId, unsigned long orderId);
	Respond receiveOrder(unsigned long tokenId, unsigned long orderId, unsigned long accountId);
	Respond startRepair(unsigned long tokenId, unsigned long orderId, unsigned long accountId);
	Respond endRepair(unsigned long tokenId, unsigned long orderId, unsigned long accountId, double transaction);
	Respond pay(unsigned long tokenId, unsigned long orderId, unsigned long accountId);
	Respond rejectOrder(unsigned long tokenId, unsigned long orderId, unsigned long accountId);

//	utility::string_t doUpdateServiceType(web::json::object &object);
//
//	// about order
//	utility::string_t doSubmitOrder(web::json::object &object);
//	utility::string_t doUpdateOrderState(std::map<utility::string_t, utility::string_t> instruction);
//	utility::string_t doGetOrderDetail(std::map<utility::string_t, utility::string_t> instruction);
//
//	// about get lists
//	utility::string_t doGetList(std::map<utility::string_t, utility::string_t> &instruction);
//	utility::string_t getServiceTypeList(std::weak_ptr<MerchantAccount> &account);
//	utility::string_t getMerchantList();
//	utility::string_t getApplianceTypeList();
//	utility::string_t getOrderList(std::weak_ptr<Account> &account);
//	utility::string_t getUnreceivedOrderForMerchant(std::weak_ptr<MerchantAccount> &account);
//
//	utility::string_t getOrderStateString(OrderState::States &&state);
//	unsigned long toUnsignedLong(const std::string &s);
//	double toDouble(const std::string &s);

	std::unique_ptr<OrderManagerSubject> orderManagerProxy;
	std::unique_ptr<AccountManagerSubject> accountManagerProxy;
	std::unique_ptr<TokenManagerSubject> tokenManagerProxy;
	std::unique_ptr<AuthenticationCarrier> authenticationCarrier;
};

#endif //HARSERVER_RESPONDGENERATOR_H
