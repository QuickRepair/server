#ifndef HAR_USERFACTORY_H
#define HAR_USERFACTORY_H

#include "Database/DatabaseConnection.hpp"
#include <string>
#include <memory>
#include <tuple>
#include <list>
#include "../Account/ContactInformation.h"
#include "../Errors/PasswordNotRightError.h"
#include "../Errors/NoSuchAnAccountError.h"

template<typename UserType>
class UserFactory {
public:
	static std::shared_ptr<UserType> readUser(std::string userName, std::string password)
	{
		auto info = DatabaseConnection::getInstance().checkPasswordAndGetUserInfo(userName, password);
		std::shared_ptr<UserType> loginUser = std::make_shared<UserType>(std::get<0>(info), userName, password);

		auto contactInformation = DatabaseConnection::getInstance().queryContactInfoByUserId(loginUser->id());
		std::list<std::shared_ptr<ContactInformation>> contact;
		//TODO, for loop, move addressInfo into addresses
		loginUser->loadContactInformation(contact);
		return loginUser;
	}

	static std::shared_ptr<UserType> createOrUpdateUser(std::string userName, std::string password)
	{
		// create or update
		try {
			DatabaseConnection::getInstance().createUserAndGenerageUserId<UserType>(userName, password);
		} catch (AccountAlreadyExistError &e) {
			DatabaseConnection::getInstance().updateUserPassword(userName, password);
		}

		// load user
		return readUser(userName, password);
	}
};

#endif //HAR_USERFACTORY_H
