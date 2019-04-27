//
// Created by gaojian on 19-4-24.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationSaver.h"
#include "Errors/NoSuchAnAccountError.hpp"
#include "Errors/QueryResultEmptyError.hpp"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"

using std::shared_ptr;			using std::make_shared;
using std::string;				using std::ostringstream;

class ManagerTest : public testing::Test {
protected:
	static void SetUpTestCase() {
		proxy = make_shared<AccountManagerProxy>();
		authentication = make_shared<AuthenticationSaver>();
		proxy->registerAuthenticationCarrier(authentication);
	}

	static void TearDownTestCase() {}

	static shared_ptr<AccountManagerProxy> proxy;
	static shared_ptr<AuthenticationSaver> authentication;
};

shared_ptr<AccountManagerProxy> ManagerTest::proxy;
shared_ptr<AuthenticationSaver> ManagerTest::authentication;

TEST_F(ManagerTest, AccountManagerForMerchantTest)
{
	EXPECT_EQ(0, proxy->getMerchantList().size());

	string account = "1000";
	string password;
	proxy->merchantRequestForVerificationCode(account);
	ostringstream os;
	os << authentication->lastAuthenticationCode();
	password = os.str();
	EXPECT_EQ(1, proxy->getMerchantList().size());
	proxy->merchantAuthentication(account, password);
	EXPECT_EQ(1, proxy->getMerchantList().size());

	auto getMerchantByAccount = proxy->getMerchant(account);
	auto getMerchantById = proxy->getMerchant(getMerchantByAccount.lock()->id());
	EXPECT_EQ(getMerchantById.lock(), getMerchantById.lock());
	EXPECT_EQ(getMerchantById.lock()->account(), account);

	auto loadedMerchant = proxy->getOrLoadMerchant(getMerchantById.lock()->id());
	EXPECT_EQ(1, proxy->getMerchantList().size());
	EXPECT_EQ(getMerchantById.lock()->id(), loadedMerchant.lock()->id());

	auto getMerchantByAccountWithWrongAccount = proxy->getMerchant("123434wetf");
	auto getMerchantByIdWithWrongId = proxy->getMerchant(1231242);
	EXPECT_TRUE(getMerchantByAccountWithWrongAccount.expired());
	EXPECT_TRUE(getMerchantByIdWithWrongId.expired());

	account = "123483274023";
	password = "9237410324712034";
	EXPECT_THROW(proxy->merchantAuthentication(account, password), NoSuchAnAccountError);

	EXPECT_THROW(proxy->getOrLoadMerchant(12341), QueryResultEmptyError);
}

TEST_F(ManagerTest, AccountManagerForCustomerTest)
{
	string account = "1111";
	string password;
	proxy->customerRequestForVerificationCode("1111");
	ostringstream os;
	os << authentication->lastAuthenticationCode();
	password = os.str();
	auto authenticationCustomer = proxy->customerAuthentication(account, password);
	auto getCustomerWithId = proxy->getCustomer(authenticationCustomer.lock()->id());
	auto getCustomerWithAccount = proxy->getCustomer(account);
	auto loadCustomer = proxy->getOrLoadCustomer(getCustomerWithId.lock()->id());
	EXPECT_EQ(authenticationCustomer.lock(), getCustomerWithId.lock());
	EXPECT_EQ(authenticationCustomer.lock(), getCustomerWithAccount.lock());
	EXPECT_EQ(authenticationCustomer.lock(), loadCustomer.lock());

	account = "123483274023";
	password = "9237410324712034";
	EXPECT_THROW(proxy->customerAuthentication(account, password), NoSuchAnAccountError);

	EXPECT_THROW(proxy->getOrLoadCustomer(12341), QueryResultEmptyError);
}

#endif