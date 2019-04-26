//
// Created by gaojian on 18-12-31.
//

#ifndef HAR_QUERYRESULT_H
#define HAR_QUERYRESULT_H

#include <mysql/mysql.h>
#include <vector>
#include <string>

/// @brief A RAII packaged MYSQL_RES
class QueryResult {
public:
	QueryResult(MYSQL_RES *res);
	~QueryResult();

	/// @brief Get a row of result
	std::vector<std::string> fetchRow();

	/// @brief Get all rows of result
	std::vector<std::string> fetchAllRows();

private:
	MYSQL_RES *m_result;
};

#endif //HAR_QUERYRESULT_H
