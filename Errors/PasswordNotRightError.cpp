//
// Created by gaojian on 19-3-14.
//

#include "PasswordNotRightError.h"

PasswordNotRightError::PasswordNotRightError(std::string error)
    : m_msg{error}
{}

const char* PasswordNotRightError::what() const noexcept
{
    return m_msg.data();
}