#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <list>
#include <memory>

class ContactInformation;
class Order;

/// @brief the super class of all account(CustomerAccount and MerchantAccount)
class Account
{
public:
	enum class Type {
		Merchant, Customer
	};
	/// @param id: the unique identity of account, generated by system
	/// @param account: the phone number of an account
	/// @param password: password of the account
    Account(unsigned long id);
    virtual ~Account() = default;

    /// @brief Test if the given order belong to the account
    /// @return true if the order belong to the account, false if not
//	virtual bool isMyOrder(std::weak_ptr<Order> order) const = 0;

	/// @brief setters and getters
	unsigned long id() const;
	const std::string password() const;
	void setPassword(const std::string &password);
	const std::string account() const;
	void setAccount(const std::string &userName);
	const std::string name() const;
	void setName(const std::string &name);

protected:
	/// @brief load contanct information for the account, can only be called by factories
//	virtual void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) = 0;

	/// @brief load orders belong to the account, can only be called by factories
//	virtual void loadOrder(std::shared_ptr<Order> order) = 0;

    unsigned long int m_id;
//	std::string m_account;
//    std::string m_password;
};

#endif // ACCOUNT_H
