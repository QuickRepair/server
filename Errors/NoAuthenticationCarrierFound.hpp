//
// Created by gaojian on 19-4-24.
//

#ifndef HARSERVER_NOAUTHENTICATIONCARRIERFOUND_HPP
#define HARSERVER_NOAUTHENTICATIONCARRIERFOUND_HPP

#include <exception>
#include <string>

/// @brief Throw this when some error happen to Database
class NoAuthenticationCarrierFound : public std::exception {
public:
	explicit NoAuthenticationCarrierFound(std::string error)
			: m_msg{std::move(error)}
	{}

	~NoAuthenticationCarrierFound() override = default;

	const char *what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};

#endif //HARSERVER_NOAUTHENTICATIONCARRIERFOUND_HPP
