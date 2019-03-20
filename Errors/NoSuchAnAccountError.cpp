//
// Created by gaojian on 19-3-14.
//

#include "NoSuchAnAccountError.h"

NoSuchAnAccountError::NoSuchAnAccountError(std::string error)
    : m_msg{std::move(error)}
{}

const char* NoSuchAnAccountError::what() const noexcept
{
    return m_msg.data();
}