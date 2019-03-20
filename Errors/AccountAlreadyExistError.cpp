//
// Created by gaojian on 19-3-20.
//

#include "AccountAlreadyExistError.h"

AccountAlreadyExistError::AccountAlreadyExistError(std::string error)
		: m_msg{std::move(error)}
{}

const char* AccountAlreadyExistError::what() const noexcept
{
	return m_msg.data();
}