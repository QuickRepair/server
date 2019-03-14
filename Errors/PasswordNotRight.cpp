//
// Created by gaojian on 19-3-14.
//

#include "PasswordNotRight.h"

PasswordNotRight::PasswordNotRight(std::string error)
    : m_msg{error}
{}

const char* PasswordNotRight::what() const noexcept
{
    return m_msg.data();
}