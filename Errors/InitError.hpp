//
// Created by gaojian on 2019/9/14.
//

#ifndef HARSERVER_INITERROR_HPP
#define HARSERVER_INITERROR_HPP

#include <exception>
#include <string>

/// @brief Throw this when a problem occurred by initial error
class InitError : public std::exception {
public:
	explicit InitError(std::string error)
			: m_msg{std::move(error)}
	{}

	~InitError() override = default;

	const char *what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};

#endif //HARSERVER_INITERROR_HPP
