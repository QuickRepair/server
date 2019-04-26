//
// Created by gaojian on 19-4-24.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h"

using std::shared_ptr;			using std::make_shared;

TEST(ManagerTest, AccountManagerTest)
{
	shared_ptr<AccountManagerProxy> proxy = make_shared<AccountManagerProxy>();
	shared_ptr<AuthenticationToScreen> authentication = make_shared<AuthenticationToScreen>();

	proxy->registerAuthenticationCarrier(authentication);

	int merchantSize = proxy->getMerchantList().size();

	//proxy->merchantRequestForVerificationCode("1000");
}

#endif