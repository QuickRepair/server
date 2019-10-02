//
// Created by gaojian on 2019/9/14.
//

#ifndef HARSERVER_DATASOURCE_HPP
#define HARSERVER_DATASOURCE_HPP

#include <memory>
#include "DataSource/DataAccess.h"
#include "Errors/InitError.hpp"

/// @brief This is a singleton class, use it to get DAO
class DataSource {
public:
	static DataAccess *getDataAccessInstance() {
		if(m_dataAccessObject)
			return m_dataAccessObject.get();
		else
			throw InitError("Data access object not initialed");
	}

private:
	DataSource() = default;
	void initDataAccessObject(std::unique_ptr<DataAccess> &&dataAccess) {
		m_dataAccessObject = std::move(dataAccess);
	}

	static std::unique_ptr<DataAccess> m_dataAccessObject;
};

#endif //HARSERVER_DATASOURCE_HPP
