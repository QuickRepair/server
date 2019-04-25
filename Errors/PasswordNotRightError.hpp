//
// Created by gaojian on 19-3-14.
//

#ifndef HARSERVER_PASSWORDNOTRIGHT_H
#define HARSERVER_PASSWORDNOTRIGHT_H

#include <exception>
#include <string>

class PasswordNotRightError : public std::exception {
public:
    explicit PasswordNotRightError(std::string error)
			: m_msg{std::move(error)}
	{}

    ~PasswordNotRightError() override = default;

    const char* what() const noexcept override {
		return m_msg.data();
	}

private:
    std::string m_msg;
};


#endif //HARSERVER_PASSWORDNOTRIGHT_H
