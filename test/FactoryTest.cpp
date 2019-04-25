//
// Created by gaojian on 19-4-11.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "ManagerProxy/AccountManagerProxy.h"
#include "Factories/DataSource/MariaDB/DatabaseConnection.h"
#include "Factories/MerchantFactory.h"
#include "Factories/CustomerFactory.h"
#include "Factories/OrderFactory.h"
#include "Errors/PasswordNotRightError.hpp"
#include "Errors/QueryResultEmptyError.hpp"
#include "Account/MerchantAccount.h"
#include "Configuration/Configure.h"

using std::make_shared;					using std::shared_ptr;
using std::string;

/* to pass the test,
 * table account in database should contain following rows
 * id	account	password	type
 * 6	1234	1234		merchant
 * 9	1		1			customer
 */

TEST(FactoryTest, merchantFactory)
{
	auto factory = make_shared<MerchantFactory>();

	// read an customer account
	string account = "1";
	string password = "1";
	EXPECT_THROW(factory->readAccount(account, password), PasswordNotRightError);

	// read merchant account
	account = "1234";
	password = "1234";
	auto merchant1 = factory->readAccount(account, password);
	EXPECT_EQ(account, merchant1->account());
	EXPECT_EQ(password, merchant1->password());
	EXPECT_TRUE(merchant1->myOrdersList().empty());

	// read merchant account with wrong password
	string wrongPassword = "";
	EXPECT_THROW(factory->readAccount(account, wrongPassword), PasswordNotRightError);

	// read merchant account with id not exist
	unsigned long id = 0;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	// read customer account with id
	id = 9;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	// read merchant account
	id = 6;
	auto merchant2 = factory->loadAccount(id);
	EXPECT_EQ(id, merchant2->id());

	// create or update merchant account
	account = "12345";
	password = "12345";
	auto merchant3 = factory->createOrUpdateAccount(account, password);
	EXPECT_EQ(account, merchant3->account());
	EXPECT_EQ(password, merchant3->password());
}

TEST(FactoryTest, customerFactory)
{
	auto factory = make_shared<CustomerFactory>();

	// read an merchant account
	string account = "1234";
	string password = "1234";
	EXPECT_THROW(factory->readAccount(account, password), PasswordNotRightError);

	// read customer account
	account = "1";
	password = "1";
	auto customer1 = factory->readAccount(account, password);
	EXPECT_EQ(account, customer1->account());
	EXPECT_EQ(password, customer1->password());
	EXPECT_TRUE(customer1->myOrdersList().empty());

	// read customer with id not exist
	unsigned long id = 0;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	// read merchant account
	id = 6;
	EXPECT_THROW(factory->loadAccount(id), QueryResultEmptyError);

	// read customer with id
	id = 9;
	auto customer2 = factory->loadAccount(id);
	EXPECT_EQ(id, customer2->id());

	// create or update customer account
	account = "2";
	password = "2";
	auto customer3 = factory->createOrUpdateAccount(account, password);
	EXPECT_EQ(account, customer3->account());
	EXPECT_EQ(password, customer3->password());
}

/* to pass the test,
 * table orders in database should contain following rows
 * id	committer	accpetor	appliance_type	detail	current_state
 * 34	1			1234		type			detail	3
 *
 * table order_states should contain following rows
 * order_id	id	last_state	date					transaction	state_name
 * 34		3	null		2019-04-13 04:04:45		null		unreceived
 */

TEST(FactoryTest, OrderFactoryTest)
{
	auto orderFactory = make_shared<OrderFactory>();
	auto accountManagerProxy = make_shared<AccountManagerProxy>();

	// merchant order
	string account = "1234";
	string password = "1234";
	auto merchant = accountManagerProxy->merchantAuthentication(account, password);
	orderFactory->getOrdersListForAccount(merchant);
	EXPECT_EQ(1, merchant.lock()->myOrdersList().size());

	//auto customerFactory = make_shared<CustomerFactory>();*/
}
#endif
