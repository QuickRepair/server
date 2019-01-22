#ifndef USER_H
#define USER_H

#include <string>
#include <list>
#include <memory>

typedef int StateFlag;

class AddressInformation;
class Order;

class User
{
public:
    User(unsigned long id, std::string name, std::string password, std::string email);

	void setId(unsigned long id);
	void setName(const std::string &name);
	void setPasswd(const std::string &passwd);
	void setEmail(const std::string &email);
	void setState(const StateFlag &state);

    unsigned long id() const;
	std::string name() const;
	std::string passwd() const;
	std::string email() const;
	StateFlag state() const;

	virtual bool isMyOrder(std::weak_ptr<Order> order) const = 0;
	virtual std::list<std::shared_ptr<Order>> myOrders() const = 0;

protected:
	virtual void loadAddress(std::list<std::shared_ptr<AddressInformation>> address) = 0;
	virtual void loadOrders(std::list<std::shared_ptr<Order>> orders) = 0;

    unsigned long int m_id;
    std::string m_name;
    std::string m_passwd;
    std::string m_email;
    StateFlag m_state;
};

#endif // USER_H
