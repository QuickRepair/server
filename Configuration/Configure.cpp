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

	mIp = pt.get<string>("listen_ip");
	mPort = pt.get<unsigned>("listen_port");

	ptree databasePt = pt.get_child("database");
	mDatabaseIp = databasePt.get<string>("ip");
	mDatabasePort = databasePt.get<unsigned>("port");
	mDatabaseName = databasePt.get<string>("database_name");
	mDatabaseUserName = databasePt.get<string>("user_name");
	mDatabasePassword = databasePt.get<string>("password");
}

std::string Configure::ip() const
{
	return mIp;
}

unsigned Configure::port() const
{
	return mPort;
}

std::string Configure::listenOn() const
{
	return (mIp + ":" + std::to_string(mPort));
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
