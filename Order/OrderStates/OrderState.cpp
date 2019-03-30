//
// Created by gaojian on 19-3-3.
//

#include "OrderState.h"

OrderState::OrderState(std::weak_ptr<Order> order, std::chrono::system_clock::time_point timePoint)
	: m_order{order}, m_stateChangeDate{timePoint}
{}

OrderState::~OrderState()
{}