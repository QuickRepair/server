#include "QueryResult.h"

using std::vector;				using std::string;

QueryResult::QueryResult(MYSQL_RES *res)
	: m_result{res}
{}

QueryResult::~QueryResult()
{
	mysql_free_result(m_result);
}

std::vector<std::string> QueryResult::fetchRow()
{
	unsigned rowLenth = mysql_num_fields(m_result);
	MYSQL_ROW mysql_row = mysql_fetch_row(m_result);
	std::vector<std::string> row;
	for(unsigned i = 0; mysql_row && i < rowLenth; ++i)
	{
		if (mysql_row[i])
			row.emplace_back(mysql_row[i]);
		else
			row.emplace_back("");
	}
	return std::move(row);
}

std::vector<std::string> QueryResult::fetchAllRows()
{
	vector<string> ret;
	while(MYSQL_ROW mysql_row = mysql_fetch_row(m_result))
	{
		unsigned rowLenth = mysql_num_fields(m_result);
		for (unsigned i = 0; mysql_row && i < rowLenth; ++i) {
			if (mysql_row[i])
				ret.emplace_back(mysql_row[i]);
			else
				ret.emplace_back("");
		}
	}
	return ret;
}