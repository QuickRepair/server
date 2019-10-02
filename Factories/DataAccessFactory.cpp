//
// Created by gaojian on 2019/9/14.
//

#include "DataAccessFactory.h"
#include "Configuration/Configure.h"
#include "DataSource/Postgres/PostgresConnection.h"

std::unique_ptr<DataAccess> DataAccessFactory::getDataAccessObject()
{
	// TODO: support dynamic DAO creation
	Configure configure;
	return std::move(std::make_unique<PostgresConnection>(
			configure.databaseName(), configure.databaseUserName(), configure.databasePassword(), configure.databaseIp(), configure.databasePort())
	);
}
