#include "Account.h"
#include "DataSource/DataAccess.h"
#include "DataSource/DataSource.hpp"

using std::string;

Account::Account(unsigned long id)
    : m_id{id}/*, m_account{std::move(account)}, m_password{std::move(password)}*/
{}

unsigned long Account::id() const {
    return m_id;
}

const string Account::password() const {
//    return m_password;
	return DataSource::getDataAccessInstance()->getPasswordFor(m_id);
}

void Account::setPassword(const string &password) {
	DataSource::getDataAccessInstance()->setPasswordFor(m_id, password);
//    m_password = password;
}

const string Account::account() const {
	return DataSource::getDataAccessInstance()->getAccountFor(m_id);
//    return m_account;
}

void Account::setAccount(const string &userName) {
	DataSource::getDataAccessInstance()->setAccountFor(m_id, userName);
//    m_account = userName;
}
