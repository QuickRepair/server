//
// Created by gaojian on 19-3-21.
//

#ifndef HARSERVER_CONFIGURE_H
#define HARSERVER_CONFIGURE_H

#include <boost/property_tree/json_parser.hpp>

class Configure {
public:
	explicit Configure(std::string configureFilePath);

	std::string ip() const;
	unsigned port() const;
	std::string listenOn() const;

	std::string databaseIp() const;
	unsigned databasePort() const;
	std::string databaseName() const;
	std::string databaseUserName() const;
	std::string databasePassword() const;

private:
	std::string mIp;
	unsigned mPort;
	std::string mDatabaseIp;
	unsigned mDatabasePort;
	std::string mDatabaseName;
	std::string mDatabaseUserName;
	std::string mDatabasePassword;
};


#endif //HARSERVER_CONFIGURE_H
