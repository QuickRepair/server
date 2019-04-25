//
// Created by gaojian on 19-3-20.
//

#ifndef HARSERVER_ACCOUNTALREADYEXISTERROR_HPP
#define HARSERVER_ACCOUNTALREADYEXISTERROR_HPP

#include <exception>
#include <string>

class AccountAlreadyExistError : public std::exception {
public:
	explicit AccountAlreadyExistError(std::string error)
			: m_msg{std::move(error)}
	{}

	~AccountAlreadyExistError() override = default;

	const char* what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};


#endif //HARSERVER_ACCOUNTALREADYEXISTERROR_HPP
