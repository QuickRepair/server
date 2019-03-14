//
// Created by gaojian on 19-3-3.
//

#include "ResultEmpty.h"

QueryResultEmpty::QueryResultEmpty(std::string error)
	: m_msg{std::move(error)}
{}

const char* QueryResultEmpty::what() const noexcept
{
	return m_msg.data();
}