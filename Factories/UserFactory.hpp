#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include "Database/DatabaseConnection.h"
#include <string>
#include <memory>
#include <tuple>
#include <list>
#include "../Account/ContactInformation.h"
#include "../Errors/PasswordNotRight.h"
#include "../Errors/NoSuchAnAccount.h"

template<typename UserType>
class UserFactory {
public:
	static std::shared_ptr<UserType> readUser(std::string userName, std::string password)
	{
		auto info = DatabaseConnection::getInstance().checkPasswordAndGetUserInfo(userName, password);
		if(std::get<2>(info) != password && std::get<3>(info) != userName)
			return nullptr;
		std::shared_ptr<UserType> loginUser = std::make_shared<UserType>(std::get<0>(info), password, userName);

		auto addressInfo = DatabaseConnection::getInstance().queryUserAddressByUserId(loginUser->id());
		std::list<std::shared_ptr<ContactInformation>> contact;
		//TODO, for loop, move addressInfo into addresses
		loginUser->loadContactInformation(contact);
		return loginUser;
	}

	static std::shared_ptr<UserType> createUser(std::string password, std::string userName)
	{
		//TODO: save to Database and get id from Database
		unsigned long id;
		return std::make_shared<UserType>(id, password, userName);
	}
};

#endif //HAR_USERFACTORY_H
