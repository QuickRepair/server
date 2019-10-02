//
// Created by gaojian on 19-4-7.
//

#ifdef ENV_TEST

#include <memory>
#include <string>
#include <gtest/gtest.h>
#include "Account/CustomerAccount.h"
#include "Account/MerchantAccount.h"
#include "Account/ContactInformation.h"
#include "Order/Order.h"
#include "Errors/OrderNotAtRightState.hpp"

using std::shared_ptr;				using std::make_shared;
using std::string;					using std::dynamic_pointer_cast;
using std::weak_ptr;

class OrderProcessWithAccount : public testing::Test {
protected:
	void SetUp() override {
		customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
		merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
		ContactInformation contactInformation;
		order = make_shared<Order>(1, customer, merchant, string("type"), contactInformation, string("detail"));
		AcceptableOrderPriceRange range;
		order->initOrderState(range);
	}

	void TearDown() override {}

	shared_ptr<CustomerAccount> customer;
	shared_ptr<MerchantAccount> merchant;
	shared_ptr<Order> order;
};

TEST_F(OrderProcessWithAccount, publishOrder)
{
	// before publish
	EXPECT_FALSE(customer->isMyOrder(order));

	// publish an order
	customer->iAmPublishAnOrder(order);
	EXPECT_EQ(1, customer->myOrdersList().size());

	// publish the order again
	customer->iAmPublishAnOrder(order);
	EXPECT_EQ(1, customer->myOrdersList().size());

	// the order in customer's order list
	EXPECT_TRUE(customer->isMyOrder(order));

	// do operations not fit current state
	EXPECT_THROW(customer->payTheOrder(order), OrderNotAtRightState);
	EXPECT_THROW(customer->evaluateTheOrder(order, OrderEvaluate()), OrderNotAtRightState);

	// the order state
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());
}

TEST_F(OrderProcessWithAccount, registerAsUnreceived)
{
	customer->iAmPublishAnOrder(order);

	// add order to merchant's pending received list
	merchant->orderWaitToBeAccept(order);

	// is the order in list
	EXPECT_FALSE(merchant->isMyOrder(order));
	EXPECT_TRUE(merchant->isMyUnreceivedOrder(order));
	EXPECT_EQ(1, merchant->myUnreceivedOrderList().size());

	// the order state
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());

	// add again and check if add twice
	merchant->orderWaitToBeAccept(order);
	EXPECT_EQ(1, merchant->myUnreceivedOrderList().size());
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());
}

TEST_F(OrderProcessWithAccount, accept)
{
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);

	// accept the order
	merchant->acceptOrder(order);
	EXPECT_TRUE(merchant->isMyOrder(order));
	EXPECT_FALSE(merchant->isMyUnreceivedOrder(order));

	// accept again
	merchant->acceptOrder(order);
	EXPECT_EQ(1, merchant->myOrdersList().size());

	// reject
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::receivedState, order->currentState());

	// do operations not fit current state
	EXPECT_THROW(merchant->endRepair(order, 1), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::receivedState, order->currentState());
}

TEST_F(OrderProcessWithAccount, reject)
{
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);

	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
	EXPECT_FALSE(merchant->isMyUnreceivedOrder(order));
	EXPECT_TRUE(merchant->isMyOrder(order));

	// do operations not fit current state
	merchant->rejectOrder(order);
	merchant->acceptOrder(order);
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_THROW(merchant->endRepair(order, 1), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
}

TEST_F(OrderProcessWithAccount, startRepair)
{
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);

	merchant->startRepair(order);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());

	// do operations not fit current state
	merchant->acceptOrder(order);
	EXPECT_EQ(1, merchant->myOrdersList().size());
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
}

TEST_F(OrderProcessWithAccount, endRepair)
{
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);
	merchant->startRepair(order);

	merchant->endRepair(order, 1);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());

	// do operations not fit current state
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	EXPECT_THROW(merchant->endRepair(order, 2), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
}

TEST_F(OrderProcessWithAccount, pay)
{
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);
	merchant->startRepair(order);
	merchant->endRepair(order, 1);

	customer->payTheOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());

	// do operations not fit current state
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_THROW(merchant->endRepair(order, 1), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
}

TEST(MerchantToMerchantOrder, merchantPublishOrder)
{
	auto merchant1 = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	auto merchant2 = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto merchantToMerchantOrder = make_shared<Order>(1, merchant1, merchant2, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	merchantToMerchantOrder->initOrderState(range);

	merchant1->iAmPublishAnOrder(merchantToMerchantOrder);
	EXPECT_TRUE(merchant1->isMyOrder(merchantToMerchantOrder));
	EXPECT_TRUE(merchant1->isMySubmittedOrder(merchantToMerchantOrder));
	EXPECT_FALSE(merchant1->isMyProcessedOrder(merchantToMerchantOrder));
	EXPECT_FALSE(merchant1->isMyUnreceivedOrder(merchantToMerchantOrder));

	merchant2->orderWaitToBeAccept(merchantToMerchantOrder);
	EXPECT_FALSE(merchant2->isMyOrder(merchantToMerchantOrder));
	EXPECT_FALSE(merchant2->isMySubmittedOrder(merchantToMerchantOrder));
	EXPECT_FALSE(merchant2->isMyProcessedOrder(merchantToMerchantOrder));
	EXPECT_TRUE(merchant2->isMyUnreceivedOrder(merchantToMerchantOrder));
}

#endif