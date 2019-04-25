//
// Created by gaojian on 19-3-30.
//

#ifndef HARSERVER_ORDERNOTATRIGHTSTATE_HPP
#define HARSERVER_ORDERNOTATRIGHTSTATE_HPP

#include <exception>
#include <string>

class OrderNotAtRightState : public std::exception {
public:
	explicit OrderNotAtRightState(std::string error)
			: m_msg{std::move(error)}
	{}

	~OrderNotAtRightState() override = default;

	const char* what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};


#endif //HARSERVER_ORDERNOTATRIGHTSTATE_HPP
