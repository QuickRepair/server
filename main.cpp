#ifdef ENV_TEST

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else

#include <iostream>
#include <memory>
#include "Configuration/Configure.h"
#include "Managers/AccountManager.h"
#include "Managers/OrderManager.h"
#include "RestServer/RestHandler.h"
#include "Managers/AuthenticationCarrier/AuthenticationToScreen.h"
#include "Factories/Database/DatabaseConnection.h"

using std::shared_ptr;			using std::make_shared;
using std::cout;				using std::runtime_error;

int main()
{
	try
	{
		Configure configure;
		DatabaseConnection::getInstance().connect(
				configure.databaseIp(), configure.databaseName(),
				configure.databaseUserName(), configure.databasePassword(),
				configure.databasePort()
		);
		AccountManager::getInstance().registerAuthenticationCarrier(make_shared<AuthenticationToScreen>());
		RestHandler handler(configure.listenOn());
		while (true)
			continue;
	}
	catch(runtime_error &e)
	{
		cout << e.what();
	}

	return 0;
}

#endif