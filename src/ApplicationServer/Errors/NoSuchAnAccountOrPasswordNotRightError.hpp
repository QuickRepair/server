//
// Created by gaojian on 19-3-14.
//

#ifndef HARSERVER_NOSUCHANACCOUNT_H
#define HARSERVER_NOSUCHANACCOUNT_H

#include <exception>
#include <string>

/// @brief Throw this when Database did not find the account with password
class NoSuchAnAccountOrPasswordNotRightError : public std::exception {
public:
    explicit NoSuchAnAccountOrPasswordNotRightError(std::string error)
			: m_msg{std::move(error)}
	{}

    ~NoSuchAnAccountOrPasswordNotRightError() override = default;

    const char* what() const noexcept override {
		return m_msg.data();
	}

private:
    std::string m_msg;
};


#endif //HARSERVER_NOSUCHANACCOUNT_H
