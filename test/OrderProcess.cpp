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
#include "Errors/OrderNotAtRightState.h"

using std::shared_ptr;				using std::make_shared;
using std::string;					using std::dynamic_pointer_cast;
using std::weak_ptr;

TEST(OrderProcess, publishOrder)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);

	customer->iAmPublishAnOrder(order);
	EXPECT_EQ(1, customer->myOrdersList().size());
	customer->iAmPublishAnOrder(order);
	EXPECT_EQ(1, customer->myOrdersList().size());
	EXPECT_TRUE(customer->isMyOrder(order));
	EXPECT_THROW(customer->payTheOrder(order), OrderNotAtRightState);
	EXPECT_THROW(customer->evaluateTheOrder(order, OrderEvaluate()), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());
}

TEST(OrderProcess, registerAsUnreceived)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);

	merchant->orderWaitToBeAccept(order);
	EXPECT_FALSE(merchant->isMyOrder(order));
	EXPECT_TRUE(merchant->isMyUnreceivedOrder(order));
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());
	merchant->orderWaitToBeAccept(order);
	EXPECT_EQ(1, merchant->myUnreceivedOrderList().size());
	EXPECT_EQ(OrderState::States::unreceivedState, order->currentState());
}

TEST(OrderProcess, accept)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);

	merchant->acceptOrder(order);
	EXPECT_TRUE(merchant->isMyOrder(order));
	EXPECT_FALSE(merchant->isMyUnreceivedOrder(order));
	merchant->acceptOrder(order);
	EXPECT_EQ(1, merchant->myOrdersList().size());
	EXPECT_THROW(merchant->endRepair(order, 1), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::receivedState, order->currentState());
}

TEST(OrderProcess, reject)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);

	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
	EXPECT_FALSE(merchant->isMyUnreceivedOrder(order));
	EXPECT_FALSE(merchant->isMyOrder(order));
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
	merchant->startRepair(order);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
	merchant->endRepair(order, 1);
	EXPECT_EQ(OrderState::States::rejectState, order->currentState());
}

TEST(OrderProcess, startRepair)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);

	merchant->startRepair(order);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(1, merchant->myOrdersList().size());
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::startRepairState, order->currentState());
}

TEST(OrderProcess, endRepair)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);
	merchant->startRepair(order);

	merchant->endRepair(order, 1);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
	EXPECT_THROW(merchant->endRepair(order, 2), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::endRepairState, order->currentState());
}

TEST(OrderProcess, pay)
{
	auto customer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
	auto merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
	ContactInformation contactInformation;
	auto order = make_shared<Order>(1, customer, string("type"), contactInformation, string("detail"));
	AcceptableOrderPriceRange range;
	order->initOrderState(range);
	customer->iAmPublishAnOrder(order);
	merchant->orderWaitToBeAccept(order);
	merchant->acceptOrder(order);
	merchant->startRepair(order);
	merchant->endRepair(order, 1);

	customer->payTheOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
	merchant->rejectOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
	merchant->acceptOrder(order);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
	EXPECT_THROW(merchant->startRepair(order), OrderNotAtRightState);
	EXPECT_THROW(merchant->endRepair(order, 1), OrderNotAtRightState);
	EXPECT_EQ(OrderState::States::finishedState, order->currentState());
}

#endif