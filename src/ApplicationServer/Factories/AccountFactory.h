#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include <string>
#include <memory>
#include <unordered_map>
#include "types.h"

class Account;
class MerchantAccount;

/// @brief The abstract factory for account factories
class AccountFactory {
public:
	virtual ~AccountFactory() = 0;

	/// @brief Read and create account from database
	/// @param account: account for the account
	/// @param password: password for the account
	/// @return a shared_ptr to Account
	virtual std::unique_ptr<Account> loadAccount(std::string account, std::string password) = 0;
	virtual void buildAccountConnection(Account *account) = 0;

	/// @brief Load a account without password
	/// @param id: id for the account
	/// @return a shared_ptr to Account
//	virtual std::shared_ptr<Account> loadAccount(unsigned long id) = 0;

	/// @brief create or update an account
	/// @param account: account to be updated or created
	/// @param password: new password
	/// @return a shared_ptr to Account just created or updated
	virtual std::unique_ptr<Account> createOrUpdateAccount(std::string account, std::string password) = 0;

protected:
	/// @brief create account with details
//	virtual void createAccountSpecific(std::string account, std::string password) = 0;
	/// @brief update account with details
//	virtual void updateAccountSpecific(std::string account, std::string password) = 0;
};

class CustomerFactory : public AccountFactory {
public:
	~CustomerFactory() override = default;
	/// @override
	std::unique_ptr<Account> loadAccount(std::string account, std::string password) override;
	std::unique_ptr<Account> createOrUpdateAccount(std::string account, std::string password) override;
	void buildAccountConnection(Account *account) override;
	/// @override
//	std::shared_ptr<Account> loadAccount(unsigned long id) override;
};

class MerchantFactory : public AccountFactory {
public:
	~MerchantFactory() override = default;

	std::unordered_map<AccountId, std::unique_ptr<MerchantAccount>> loadAllMerchants();

	/// @override
	std::unique_ptr<Account> loadAccount(std::string account, std::string password) override;
	std::unique_ptr<Account> createOrUpdateAccount(std::string account, std::string password) override;
	void buildAccountConnection(Account *account) override;

	/// @brief Call this function to persistence merchant service types
	/// @param merchant: a weak_ptr to related merchant
//	void persistenceSupportedServices(std::weak_ptr<MerchantAccount> merchant);
};

#endif //HAR_USERFACTORY_H
