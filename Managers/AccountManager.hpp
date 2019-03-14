//
// Created by gaojian on 19-2-26.
//

#ifndef HAR_ACCOUNTMANAGER_H
#define HAR_ACCOUNTMANAGER_H

#include <list>
#include <memory>
#include <string>
#include "../Errors/NoSuchAnAccount.h"
#include "../Errors/PasswordNotRight.h"

class Account;
class MerchantAccount;
class CustomerAccount;
template<typename UserType>
class UserFactory;

class AccountManager {
public:
	static AccountManager &getInstance();

	std::list<std::weak_ptr<MerchantAccount>> getMerchantList();
	void requestForVerificationCode(std::string codeSendToWhere);

	template<typename AccountType>
	bool accountAuthentication(std::string userName, std::string password) {
        std::shared_ptr<AccountType> account;
        try {
            account = UserFactory<AccountType>::readUser(userName, password);
        } catch (NoSuchAnAccount &e) {
            account = UserFactory<AccountType>::createUser(userName, password);
        } catch (PasswordNotRight &e) {
            return false;
        }

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
	    return true;
	}

private:
	AccountManager();

	std::list<std::shared_ptr<MerchantAccount>> m_merchantAccount;
	std::list<std::shared_ptr<CustomerAccount>> m_customerAccount;
};

#endif //HAR_ACCOUNTMANAGER_H
