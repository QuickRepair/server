#include "Account.h"

using std::string;

Account::Account(unsigned long id, std::string account, std::string password)
    : m_id{id}, m_account{std::move(account)}, m_password{std::move(password)}
{}

unsigned long Account::id() const {
    return m_id;
}

void Account::setId(unsigned long id) {
    m_id = id;
}

const string &Account::password() const {
    return m_password;
}

void Account::setPassword(const string &password) {
    m_password = password;
}

const string &Account::userName() const {
    return m_account;
}

void Account::setUserName(const string &userName) {
    m_account = userName;
}
