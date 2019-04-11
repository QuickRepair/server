//
// Created by gaojian on 19-4-11.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "Factories/Database/DatabaseConnection.h"
#include "Factories/MerchantFactory.h"
#include "Factories/CustomerFactory.h"
#include "Errors/PasswordNotRightError.h"
#include "Errors/QueryResultEmptyError.h"
#include "Account/MerchantAccount.h"
#include "Configuration/Configure.h"

using std::make_shared;					using std::shared_ptr;
using std::string;

/* to pass the test,
 * table account in database should contain following rows
   id	account	password	type
   6	1234	1234		merchant
   9	1		1			customer
*/

TEST(FactoryTest, merchantFactory)
{
	Configure configure("./har.json");
	DatabaseConnection::getInstance().connect(
			configure.databaseIp(), configure.databaseName(),
			configure.databaseUserName(), configure.databasePassword(),
			configure.databasePort()
	);

	auto factory = make_shared<MerchantFactory>();

	string account = "1";
	string password = "1";
	EXPECT_THROW(factory->readAccount(account, password), PasswordNotRightError);

	account = "1234";
	password = "1234";
	auto merchant1 = factory->readAccount(account, password);
	EXPECT_EQ(account, merchant1->account());
	EXPECT_EQ(password, merchant1->password());
	EXPECT_TRUE(merchant1->myOrdersList().empty());

	string wrongPassword = "";
	EXPECT_THROW(factory->readAccount(account, wrongPassword), PasswordNotRightError);

	unsigned long id = 0;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	id = 9;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	id = 6;
	auto merchant2 = factory->loadAccount(id);
	EXPECT_EQ(id, merchant2->id());

	account = "12345";
	password = "12345";
	auto merchant3 = factory->createOrUpdateAccount(account, password);
	EXPECT_EQ(account, merchant3->account());
	EXPECT_EQ(password, merchant3->password());
}

TEST(FactoryTest, customerFactory)
{
	auto factory = make_shared<CustomerFactory>();

	string account = "1234";
	string password = "1234";
	EXPECT_THROW(factory->readAccount(account, password), PasswordNotRightError);

	account = "1";
	password = "1";
	auto customer1 = factory->readAccount(account, password);
	EXPECT_EQ(account, customer1->account());
	EXPECT_EQ(password, customer1->password());
	EXPECT_TRUE(customer1->myOrdersList().empty());

	unsigned long id = 0;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	id = 6;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	id = 9;
	auto customer2 = factory->loadAccount(id);
	EXPECT_EQ(id, customer2->id());

	account = "2";
	password = "2";
	auto customer3 = factory->createOrUpdateAccount(account, password);
	EXPECT_EQ(account, customer3->account());
	EXPECT_EQ(password, customer3->password());
}
#endif
