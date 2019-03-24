#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include <string>
#include <memory>

class Account;

class AccountFactory {
public:
	virtual ~AccountFactory() = 0;

	std::shared_ptr<Account> readUser(std::string account, std::string password);
	std::shared_ptr<Account> createOrUpdateUser(std::string account, std::string password);

protected:
	virtual std::shared_ptr<Account> loadAccountSpecific(std::string account, std::string password) = 0;
	virtual void createAccountSpecific(std::string account, std::string password) = 0;
	virtual void updateAccountSpecific(std::string account, std::string password) = 0;
};

#endif //HAR_USERFACTORY_H
