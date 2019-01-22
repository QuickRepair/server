#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include "databaseconnection.h"
#include <string>
#include <memory>
#include <tuple>
#include <list>
#include "../entity/user/addressinformation.h"

class User;

template<typename UserType>
class UserFactory {
public:
	std::shared_ptr<UserType> readUser(std::string email, std::string password)
	{
		auto info = DatabaseConnection::getInstance().checkPasswordAndGetUserInfo(email, password);
		if(std::get<2>(info) != password && std::get<3>(info) != email)
			return nullptr;
		std::shared_ptr<UserType> newUser = std::make_shared<UserType>(std::get<0>(info), std::get<1>(info), password, email);

		auto addressInfo = DatabaseConnection::getInstance().queryUserAddressByUserId(newUser->id());
		std::list<std::shared_ptr<AddressInformation>> addresses;
		//TODO, for loop, move addressInfo into addresses
		newUser->loadAddress(addresses);
		return newUser;
	}

	std::shared_ptr<UserType> createUser(unsigned long id, std::string name, std::string password, std::string email)
	{
		return std::make_shared<UserType>(id, name, password, email);
	}
};

#endif //HAR_USERFACTORY_H
