//
// Created by gaojian on 19-3-30.
//

#ifndef HARSERVER_ORDERNOTATRIGHTSTATE_H
#define HARSERVER_ORDERNOTATRIGHTSTATE_H

#include <exception>
#include <string>

class OrderNotAtRightState : public std::exception {
public:
	explicit OrderNotAtRightState(std::string error);
	~OrderNotAtRightState() override = default;

	const char* what() const noexcept override;

private:
	std::string m_msg;
};


#endif //HARSERVER_ORDERNOTATRIGHTSTATE_H
