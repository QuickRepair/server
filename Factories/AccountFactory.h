#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include <string>
#include <memory>

class Account;

/// @brief The abstract factory for account factories
class AccountFactory {
public:
	virtual ~AccountFactory() = 0;

	/// @brief Read and create account from database
	/// @param account: account for the account
	/// @param password: password for the account
	/// @return a shared_ptr to Account
	std::shared_ptr<Account> readAccount(std::string account, std::string password);

	/// @brief Load a account without password
	/// @param id: id for the account
	/// @return a shared_ptr to Account
	virtual std::shared_ptr<Account> loadAccount(unsigned long id) = 0;

	/// @brief create or update an account
	/// @param account: account to be updated or created
	/// @param password: new password
	/// @return a shared_ptr to Account just created or updated
	std::shared_ptr<Account> createOrUpdateAccount(std::string account, std::string password);

protected:
	/// @brief load customer or merchant specific details
	/// @param account: account for the account
	/// @param password: password for the account
	/// @return a shared_to Account just loaded
	virtual std::shared_ptr<Account> loadAccountSpecific(std::string account, std::string password) = 0;

	/// @brief create account with details
	virtual void createAccountSpecific(std::string account, std::string password) = 0;
	/// @brief update account with details
	virtual void updateAccountSpecific(std::string account, std::string password) = 0;
};

#endif //HAR_USERFACTORY_H
