//
// Created by gaojian on 19-3-3.
//

#include "QueryResultEmptyError.h"

QueryResultEmptyError::QueryResultEmptyError(std::string error)
	: m_msg{std::move(error)}
{}

const char* QueryResultEmptyError::what() const noexcept
{
	return m_msg.data();
}