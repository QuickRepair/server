//
// Created by gaojian on 19-3-3.
//
/* Throw this when
 * query result of Database is empty */

#ifndef HAR_RESULTEMPTY_H
#define HAR_RESULTEMPTY_H

#include <exception>
#include <string>

class QueryResultEmpty : public std::exception {
public:
	explicit QueryResultEmpty(std::string error);
	~QueryResultEmpty() override = default;

	const char* what() const noexcept override;

private:
	std::string m_msg;
};


#endif //HAR_RESULTEMPTY_H
