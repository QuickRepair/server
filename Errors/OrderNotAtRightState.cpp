//
// Created by gaojian on 19-3-30.
//

#include "OrderNotAtRightState.h"

OrderNotAtRightState::OrderNotAtRightState(std::string error)
		: m_msg{error}
{}

const char* OrderNotAtRightState::what() const noexcept
{
	return m_msg.data();
}