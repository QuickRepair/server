//
// Created by gaojian on 19-2-26.
//

#ifndef HAR_ACCOUNTMANAGER_H
#define HAR_ACCOUNTMANAGER_H

#include <list>
#include <memory>
#include <string>
#include "../Errors/NoSuchAnAccountError.h"
#include "../Errors/PasswordNotRightError.h"
#include "../Account/MerchantAccount.h"
#include "../Account/CustomerAccount.h"
#include "../Factories/UserFactory.hpp"
#include "AuthenticationCarrier/AuthenticationCarrier.h"

class Account;
class MerchantAccount;
class CustomerAccount;
template<typename UserType>
class UserFactory;

class AccountManager {
public:
	static AccountManager &getInstance();

	void registerAuthenticationCarrier(std::shared_ptr<AuthenticationCarrier> carrier);

	std::list<std::weak_ptr<MerchantAccount>> getMerchantList();

	template<typename AccountType>
	void requestForVerificationCode(std::string codeSendToWhere) {
		// get verification code
		long code = m_authenticationCarrier->sendVerificationCode(codeSendToWhere);

		// try to update/create account
		std::shared_ptr<AccountType> account;
		account = UserFactory<AccountType>::createOrUpdateUser(codeSendToWhere, std::to_string(code));

		saveToList(account);
	}

	template<typename AccountType>
	bool accountAuthentication(std::string userName, std::string password) {
		//TODO: find account in list

		// try to read from database
        std::shared_ptr<AccountType> account = UserFactory<AccountType>::readUser(userName, password);
	    saveToList(account);

	    return true;
	}

private:
	AccountManager() = default;

	//save to list
	template<typename AccountType>
	void saveToList(std::shared_ptr<AccountType> account) {
		if(typeid(AccountType) == typeid(MerchantAccount)) {
			m_merchantAccount.push_back(
					[](std::shared_ptr<Account> account) {
						return std::dynamic_pointer_cast<MerchantAccount>(account);
					}(account)
			);
		}
		else if(typeid(AccountType) == typeid(CustomerAccount)) {
			m_customerAccount.push_back(
					[](std::shared_ptr<Account> account) {
						return std::dynamic_pointer_cast<CustomerAccount>(account);
					}(account)
			);
		}
	}

	std::list<std::shared_ptr<MerchantAccount>> m_merchantAccount;
	std::list<std::shared_ptr<CustomerAccount>> m_customerAccount;

	std::shared_ptr<AuthenticationCarrier> m_authenticationCarrier;
};

#endif //HAR_ACCOUNTMANAGER_H
