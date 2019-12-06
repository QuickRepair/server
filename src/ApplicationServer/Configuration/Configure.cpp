//
// Created by gaojian on 19-3-21.
//

#include "Configure.h"

using boost::property_tree::ptree;				using boost::property_tree::read_json;
using std::string;

Configure::Configure(std::string configureFilePath)
{
	ptree pt;
	read_json(configureFilePath, pt);

	mRPCAddress = pt.get<string>("rpc_server");
	mRestAddress = pt.get<string>("rest_server");

	ptree databasePt = pt.get_child("database");
	mDatabaseIp = databasePt.get<string>("ip");
	mDatabasePort = databasePt.get<unsigned>("port");
	mDatabaseName = databasePt.get<string>("database_name");
	mDatabaseUserName = databasePt.get<string>("user_name");
	mDatabasePassword = databasePt.get<string>("password");
}

std::string Configure::RPCAddress() const
{
	return mRPCAddress;
}

std::string Configure::RestAddress() const
{
	return mRestAddress;
}

std::string Configure::databaseIp() const
{
	return mDatabaseIp;
}

unsigned Configure::databasePort() const
{
	return mDatabasePort;
}

std::string Configure::databaseName() const
{
	return mDatabaseName;
}

std::string Configure::databaseUserName() const
{
	return mDatabaseUserName;
}

std::string Configure::databasePassword() const
{
	return mDatabasePassword;
}
