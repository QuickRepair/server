//
// Created by gaojian on 19-3-14.
//

#include "NoSuchAnAccount.h"

NoSuchAnAccount::NoSuchAnAccount(std::string error)
    : m_msg{std::move(error)}
{}

const char* NoSuchAnAccount::what() const noexcept
{
    return m_msg.data();
}