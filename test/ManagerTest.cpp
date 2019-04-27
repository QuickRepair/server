//
// Created by gaojian on 19-4-24.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>
#include <Order/OrderStates/OrderState.h>
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationSaver.h"
#include "Errors/NoSuchAnAccountError.hpp"
#include "Errors/QueryResultEmptyError.hpp"
#include "Account/MerchantAccount.h"
#include "Account/CustomerAccount.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include "Account/ContactInformation.h"
#include "Order/OrderStates/AcceptableOrderPriceRange.h"
#include "Order/Order.h"

using std::shared_ptr;			using std::make_shared;
using std::string;				using std::ostringstream;

class ManagerTest : public testing::Test {
protected:
	static void SetUpTestCase() {
		accountProxy = make_shared<AccountManagerProxy>();
		authentication = make_shared<AuthenticationSaver>();
		orderProxy = make_shared<OrderManagerProxy>();
		accountProxy->registerAuthenticationCarrier(authentication);
	}

	static void TearDownTestCase() {}

	static shared_ptr<AccountManagerProxy> accountProxy;
	static shared_ptr<OrderManagerProxy> orderProxy;
	static shared_ptr<AuthenticationSaver> authentication;
};

shared_ptr<AccountManagerProxy> ManagerTest::accountProxy;
shared_ptr<OrderManagerProxy> ManagerTest::orderProxy;
shared_ptr<AuthenticationSaver> ManagerTest::authentication;

TEST_F(ManagerTest, AccountManagerForMerchantTest)
{
	EXPECT_EQ(0, accountProxy->getMerchantList().size());

	string account = "1000";
	string password;
	accountProxy->merchantRequestForVerificationCode(account);
	ostringstream os;
	os << authentication->lastAuthenticationCode();
	password = os.str();
	EXPECT_EQ(1, accountProxy->getMerchantList().size());
	accountProxy->merchantAuthentication(account, password);
	EXPECT_EQ(1, accountProxy->getMerchantList().size());

	auto getMerchantByAccount = accountProxy->getMerchant(account);
	auto getMerchantById = accountProxy->getMerchant(getMerchantByAccount.lock()->id());
	EXPECT_EQ(getMerchantById.lock(), getMerchantById.lock());
	EXPECT_EQ(getMerchantById.lock()->account(), account);

	auto loadedMerchant = accountProxy->getOrLoadMerchant(getMerchantById.lock()->id());
	EXPECT_EQ(1, accountProxy->getMerchantList().size());
	EXPECT_EQ(getMerchantById.lock()->id(), loadedMerchant.lock()->id());

	auto getMerchantByAccountWithWrongAccount = accountProxy->getMerchant("123434wetf");
	auto getMerchantByIdWithWrongId = accountProxy->getMerchant(1231242);
	EXPECT_TRUE(getMerchantByAccountWithWrongAccount.expired());
	EXPECT_TRUE(getMerchantByIdWithWrongId.expired());

	account = "123483274023";
	password = "9237410324712034";
	EXPECT_THROW(accountProxy->merchantAuthentication(account, password), NoSuchAnAccountError);

	EXPECT_THROW(accountProxy->getOrLoadMerchant(12341), QueryResultEmptyError);
}

TEST_F(ManagerTest, AccountManagerForCustomerTest)
{
	string account = "1111";
	string password;
	accountProxy->customerRequestForVerificationCode("1111");
	ostringstream os;
	os << authentication->lastAuthenticationCode();
	password = os.str();
	auto authenticationCustomer = accountProxy->customerAuthentication(account, password);
	auto getCustomerWithId = accountProxy->getCustomer(authenticationCustomer.lock()->id());
	auto getCustomerWithAccount = accountProxy->getCustomer(account);
	auto loadCustomer = accountProxy->getOrLoadCustomer(getCustomerWithId.lock()->id());
	EXPECT_EQ(authenticationCustomer.lock(), getCustomerWithId.lock());
	EXPECT_EQ(authenticationCustomer.lock(), getCustomerWithAccount.lock());
	EXPECT_EQ(authenticationCustomer.lock(), loadCustomer.lock());

	account = "123483274023";
	password = "9237410324712034";
	EXPECT_THROW(accountProxy->customerAuthentication(account, password), NoSuchAnAccountError);

	EXPECT_THROW(accountProxy->getOrLoadCustomer(12341), QueryResultEmptyError);
}

TEST_F(ManagerTest, OrderManagerLoadTest)
{
	string account = "1234";
	string password = "1234";
	auto merchant = accountProxy->merchantAuthentication(account, password);
	EXPECT_EQ(0, merchant.lock()->myOrdersList().size());

	account = "1";
	password = "1";
	auto customer = accountProxy->customerAuthentication(account, password);
	EXPECT_EQ(0, customer.lock()->myOrdersList().size());

	orderProxy->loadAllOrderForAccount(merchant);
	EXPECT_NE(0, merchant.lock()->myOrdersList().size());
	EXPECT_NE(0, customer.lock()->myOrdersList().size());
}

TEST_F(ManagerTest, OrderManagerTest)
{
	string account = "1000";
	auto merchant = accountProxy->getMerchant(account);
	account = "1111";
	auto customer = accountProxy->getCustomer(account);
	string applianceType = "type";
	ContactInformation contact;
	string detail = "detail";
	AcceptableOrderPriceRange range;
	auto newOrder = orderProxy->publishOrder(customer, merchant, applianceType, contact, detail, range);
	EXPECT_TRUE(customer.lock()->isMyOrder(newOrder));
	EXPECT_TRUE(merchant.lock()->isMyUnreceivedOrder(newOrder));
	EXPECT_FALSE(merchant.lock()->isMyOrder(newOrder));
	EXPECT_EQ(OrderState::unreceivedState, newOrder.lock()->currentState());
	EXPECT_EQ(orderProxy->getOrder(newOrder.lock()->id()).lock(), newOrder.lock());

	orderProxy->orderAccepted(merchant, newOrder);
	EXPECT_TRUE(customer.lock()->isMyOrder(newOrder));
	EXPECT_FALSE(merchant.lock()->isMyUnreceivedOrder(newOrder));
	EXPECT_TRUE(merchant.lock()->isMyOrder(newOrder));
	EXPECT_EQ(OrderState::receivedState, newOrder.lock()->currentState());

	orderProxy->orderStartRepair(merchant, newOrder);
	EXPECT_EQ(OrderState::startRepairState, newOrder.lock()->currentState());

	orderProxy->orderEndRepair(merchant, newOrder, 1);
	EXPECT_EQ(OrderState::endRepairState, newOrder.lock()->currentState());
	EXPECT_EQ(1, newOrder.lock()->transaction());

	orderProxy->orderPayed(customer, newOrder);
	EXPECT_EQ(OrderState::finishedState, newOrder.lock()->currentState());

	applianceType = "type1";
	detail = "detail1";
	auto rejectOrder = orderProxy->publishOrder(customer, merchant, applianceType, contact, detail, range);
	orderProxy->orderRejected(merchant, rejectOrder);
	EXPECT_TRUE(customer.lock()->isMyOrder(rejectOrder));
	EXPECT_FALSE(merchant.lock()->isMyUnreceivedOrder(rejectOrder));
	EXPECT_TRUE(merchant.lock()->isMyOrder(rejectOrder));
	EXPECT_EQ(OrderState::rejectState, rejectOrder.lock()->currentState());
}

#endif