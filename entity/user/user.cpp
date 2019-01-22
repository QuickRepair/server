#include "user.h"

using std::string;

User::User(unsigned long id, std::string name, std::string password, std::string email)
    : m_id{id}, m_name{std::move(name)}, m_passwd{std::move(password)}, m_email{std::move(email)}
{}

unsigned long User::id() const
{
    return m_id;
}

void User::setId(unsigned long id)
{
    m_id = id;
}

string User::name() const
{
    return m_name;
}

void User::setName(const string &name)
{
    m_name = name;
}

string User::passwd() const
{
    return m_passwd;
}

void User::setPasswd(const string &passwd)
{
    m_passwd = passwd;
}

string User::email() const
{
    return m_email;
}

void User::setEmail(const string &email)
{
    m_email = email;
}

StateFlag User::state() const
{
    return m_state;
}

void User::setState(const StateFlag &state)
{
    m_state = state;
}
