//
// Created by gaojian on 19-3-3.
//


#ifndef HAR_DATABASEINTERNALERROR_H
#define HAR_DATABASEINTERNALERROR_H

#include <exception>
#include <string>

/// @brief Throw this when some error happen to Database
class DatabaseInternalError : public std::exception {
public:
	explicit DatabaseInternalError(std::string error)
			: m_msg{std::move(error)}
	{}

	~DatabaseInternalError() override = default;

	const char *what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};


#endif //HAR_DATABASEINTERNALERROR_H
