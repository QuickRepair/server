//
// Created by gaojian on 19-4-11.
//

#ifdef ENV_TEST

/// @brief In test environment, marco DATA_SOURCE_FROM is set to read data from SimulateDatabase

#include <gtest/gtest.h>
#include <memory>
#include <list>
#include "Factories/DataSource/MariaDB/MariaDBConnection.h"
#include "Factories/MerchantFactory.h"
#include "Factories/CustomerFactory.h"
#include "Factories/OrderFactory.h"
#include "Errors/PasswordNotRightError.hpp"
#include "Errors/QueryResultEmptyError.hpp"
#include "Errors/OrderNotAtRightState.hpp"
#include "Account/MerchantAccount.h"
#include "Order/Order.h"

using std::make_shared;					using std::shared_ptr;
using std::string;						using std::list;
using std::dynamic_pointer_cast;

class FactoryTest : public testing::Test {
protected:
	static void SetUpTestCase() {
		orderFactory = make_shared<OrderFactory>();
		merchantFactory = make_shared<MerchantFactory>();
		customerFactory = make_shared<CustomerFactory>();
	}

	static void TearDownTestCase() {}

	static shared_ptr<OrderFactory> orderFactory;
	static shared_ptr<MerchantFactory> merchantFactory;
	static shared_ptr<CustomerFactory> customerFactory;
};

shared_ptr<OrderFactory> FactoryTest::orderFactory;
shared_ptr<MerchantFactory> FactoryTest::merchantFactory;
shared_ptr<CustomerFactory> FactoryTest::customerFactory;

/* to pass the test,
 * table account in database should contain following rows
 * id	account	password	type
 * 6	1234	1234		merchant
 * 9	1		1			customer
 */

TEST_F(FactoryTest, merchantFactory)
{
	// read an customer account
	string account = "1";
	string password = "1";
	EXPECT_THROW(merchantFactory->readAccount(account, password), PasswordNotRightError);

	// read merchant account
	account = "1234";
	password = "1234";
	auto merchant1 = merchantFactory->readAccount(account, password);
	EXPECT_EQ(account, merchant1->account());
	EXPECT_EQ(password, merchant1->password());
	EXPECT_TRUE(merchant1->myOrdersList().empty());

	// read merchant account with wrong password
	string wrongPassword = "";
	EXPECT_THROW(merchantFactory->readAccount(account, wrongPassword), PasswordNotRightError);

	// read merchant account with id not exist
	unsigned long id = 0;
	EXPECT_THROW(merchantFactory->loadAccount(id), QueryResultEmptyError);

	// read customer account with id
	id = 9;
	EXPECT_THROW(merchantFactory->loadAccount(id), QueryResultEmptyError);

	// read merchant account
	id = 6;
	auto merchant2 = merchantFactory->loadAccount(id);
	EXPECT_EQ(id, merchant2->id());

	// create or update merchant account
	account = "12345";
	password = "12345";
	auto merchant3 = merchantFactory->createOrUpdateAccount(account, password);
	EXPECT_EQ(account, merchant3->account());
	EXPECT_EQ(password, merchant3->password());
}

TEST_F(FactoryTest, customerFactory)
{
	// read an merchant account
	string account = "1234";
	string password = "1234";
	EXPECT_THROW(customerFactory->readAccount(account, password), PasswordNotRightError);

	// read customer account
	account = "1";
	password = "1";
	auto customer1 = customerFactory->readAccount(account, password);
	EXPECT_EQ(account, customer1->account());
	EXPECT_EQ(password, customer1->password());
	EXPECT_TRUE(customer1->myOrdersList().empty());

	// read customer with id not exist
	unsigned long id = 0;
	EXPECT_THROW(customerFactory->loadAccount(id), QueryResultEmptyError);

	// read merchant account
	id = 6;
	EXPECT_THROW(customerFactory->loadAccount(id), QueryResultEmptyError);

	// read customer with id
	id = 9;
	auto customer2 = customerFactory->loadAccount(id);
	EXPECT_EQ(id, customer2->id());

	// create or update customer account
	account = "2";
	password = "2";
	auto customer3 = customerFactory->createOrUpdateAccount(account, password);
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

TEST_F(FactoryTest, OrderFactoryLoadTest)
{
	// merchant order
	string account = "1234";
	string password = "1234";
	auto merchant = merchantFactory->readAccount(account, password);
	list<shared_ptr<Order>> merchantOrderList = orderFactory->getOrdersListForAccount(merchant);
	EXPECT_EQ(0, merchant->myOrdersList().size());
	EXPECT_EQ(1, merchantOrderList.size());
	for(auto &order : merchantOrderList)
		EXPECT_EQ(merchant->id(), order->acceptorId());
	EXPECT_EQ(OrderState::unreceivedState, (*merchantOrderList.begin())->currentState());

	// customer order
	account = "1";
	password = "1";
	auto customer = customerFactory->readAccount(account, password);
	list<shared_ptr<Order>> customerOrderList = orderFactory->getOrdersListForAccount(customer);
	EXPECT_EQ(0, customer->myOrdersList().size());
	EXPECT_EQ(1, customerOrderList.size());
	for(auto &order : customerOrderList)
		EXPECT_EQ(customer->id(), order->committerId());
	auto firstOrder = *customerOrderList.begin();
	EXPECT_EQ(OrderState::unreceivedState, firstOrder->currentState());
	EXPECT_THROW(firstOrder->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(firstOrder->endRepair(1), OrderNotAtRightState);
	EXPECT_THROW(firstOrder->pay(), OrderNotAtRightState);
}

TEST_F(FactoryTest, orderFactoryCreateTest)
{
	auto customer = customerFactory->loadAccount(9);
	auto merchant = merchantFactory->loadAccount(6);
	string applianceType = "type";
	ContactInformation contact;
	string detail;
	AcceptableOrderPriceRange range;
	auto order = orderFactory->createOrder(
			dynamic_pointer_cast<CustomerAccount>(customer),
			dynamic_pointer_cast<MerchantAccount>(merchant),
			applianceType, contact, detail, range
	);
	EXPECT_FALSE(customer->isMyOrder(order));
	EXPECT_EQ(order->committerId(), customer->id());
	EXPECT_FALSE(merchant->isMyOrder(order));
	EXPECT_EQ(order->acceptorId(), merchant->id());
	EXPECT_EQ(OrderState::unreceivedState, order->currentState());
}

#endif
