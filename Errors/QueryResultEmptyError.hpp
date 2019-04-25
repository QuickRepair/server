//
// Created by gaojian on 19-3-3.
//

#ifndef HAR_RESULTEMPTY_H
#define HAR_RESULTEMPTY_H

#include <exception>
#include <string>

/// @brief Throw this when query result of Database is empty
class QueryResultEmptyError : public std::exception {
public:
	explicit QueryResultEmptyError(std::string error)
			: m_msg{std::move(error)}
	{}

	~QueryResultEmptyError() override = default;

	const char* what() const noexcept override {
		return m_msg.data();
	}

private:
	std::string m_msg;
};


#endif //HAR_RESULTEMPTY_H
