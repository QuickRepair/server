#ifndef USER_H
#define USER_H

#include <string>
#include <list>
#include <memory>

class ContactInformation;
class Order;

class Account
{
public:
    Account(unsigned long id, std::string password, std::string userName);
    virtual ~Account() = default;

	virtual bool isMyOrder(std::weak_ptr<Order> order) const = 0;
	virtual std::list<std::weak_ptr<Order>> myOrders() const = 0;

	unsigned long id() const;
	void setId(unsigned long id);
	const std::string &password() const;
	void setPassword(const std::string &password);
	const std::string &userName() const;
	void setUserName(const std::string &userName);

protected:
	virtual void loadContactInformation(std::list<std::shared_ptr<ContactInformation>> info) = 0;
	virtual void loadOrders(std::list<std::weak_ptr<Order>> orders) = 0;

    unsigned long int m_id;
    std::string m_password;
    std::string m_userName;
};

#endif // USER_H
