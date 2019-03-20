//
// Created by gaojian on 19-3-20.
//

#ifndef HARSERVER_ACCOUNTALREADYEXISTERROR_H
#define HARSERVER_ACCOUNTALREADYEXISTERROR_H

#include <exception>
#include <string>

class AccountAlreadyExistError : public std::exception {
public:
	explicit AccountAlreadyExistError(std::string error);
	~AccountAlreadyExistError() override = default;

	const char* what() const noexcept override;

private:
	std::string m_msg;
};


#endif //HARSERVER_ACCOUNTALREADYEXISTERROR_H
