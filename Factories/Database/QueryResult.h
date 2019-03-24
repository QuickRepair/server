//
// Created by gaojian on 18-12-31.
//

#ifndef HAR_QUERYRESULT_H
#define HAR_QUERYRESULT_H

#include <mariadb/mysql.h>
#include <vector>
#include <string>

class QueryResult {
public:
	QueryResult(MYSQL_RES *res);
	~QueryResult();

	std::vector<std::string> fetchRow();
	std::vector<std::string> fetchAllRows();

private:
	MYSQL_RES *m_result;
};

#endif //HAR_QUERYRESULT_H
