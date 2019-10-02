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
	std::string ip() const;
	unsigned port() const;

	/// @brief Connect ip with port
	/// @example: "localhost:8080"
	std::string listenOn() const;

	/// @brief Getters
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
