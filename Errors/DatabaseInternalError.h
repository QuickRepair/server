//
// Created by gaojian on 19-3-3.
//
/* Throw this when
 * some error happen to Database
 */

#ifndef HAR_DATABASEINTERNALERROR_H
#define HAR_DATABASEINTERNALERROR_H

#include <exception>
#include <string>

class DatabaseInternalError : public std::exception {
public:
	explicit DatabaseInternalError(std::string error);
	~DatabaseInternalError() override = default;

	const char *what() const noexcept override;

private:
	std::string m_msg;
};


#endif //HAR_DATABASEINTERNALERROR_H
