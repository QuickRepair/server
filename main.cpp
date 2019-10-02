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
#include "ManagerProxy/AccountManagerProxy.h"
#include "RestServer/RestHandler.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h"

using std::shared_ptr;			using std::make_shared;
using std::cout;				using std::runtime_error;

int main()
{
	try
	{
		Configure configure;
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