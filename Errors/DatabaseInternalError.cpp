//
// Created by gaojian on 19-3-3.
//

#include "DatabaseInternalError.h"

DatabaseInternalError::DatabaseInternalError(std::string error)
	: m_msg{std::move(error)}
{}

const char* DatabaseInternalError::what() const noexcept
{
	return m_msg.data();
}