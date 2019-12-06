//
// Created by gaojian on 19-3-21.
//

#ifndef HARSERVER_CONFIGURE_H
#define HARSERVER_CONFIGURE_H

#include <boost/property_tree/json_parser.hpp>

/// @brief Read settings from config file
class Configure {
public:
	/// @param configureFilePath: the path to config file
	explicit Configure(std::string configureFilePath = "qr.json");

	/// @brief Getters
	std::string RPCAddress() const;
	std::string RestAddress() const;

	/// @brief Getters
	std::string databaseIp() const;
	unsigned databasePort() const;
	std::string databaseName() const;
	std::string databaseUserName() const;
	std::string databasePassword() const;

private:
	std::string mRPCAddress;
	std::string mRestAddress;
	std::string mDatabaseIp;
	unsigned mDatabasePort;
	std::string mDatabaseName;
	std::string mDatabaseUserName;
	std::string mDatabasePassword;
};


#endif //HARSERVER_CONFIGURE_H
