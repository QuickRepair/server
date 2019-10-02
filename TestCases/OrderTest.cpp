//
// Created by gaojian on 19-4-26.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "Order/Order.h"
#include "Account/ContactInformation.h"
#include "Errors/OrderNotAtRightState.hpp"
#include "Account/CustomerAccount.h"
#include "Account/MerchantAccount.h"

using std::make_shared;					using std::string;
using std::shared_ptr;

class OrderTest : public testing::Test {
protected:
	void SetUp() override {
		committer = make_shared<CustomerAccount>(1, string("customer"), string("password"));
		merchant = make_shared<MerchantAccount>(2, string("merchant"), string("password"));
		ContactInformation contactInformation;
		order = make_shared<Order>(1, committer, merchant, "type", contactInformation, "detail");
		AcceptableOrderPriceRange range;
		order->initOrderState(range);
	}

	void TearDown() override {}

	shared_ptr<CustomerAccount> committer;
	shared_ptr<MerchantAccount> merchant;
	shared_ptr<Order> order;
};

TEST_F(OrderTest, unreceivedTest)
{
	EXPECT_THROW(order->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepair(1), OrderNotAtRightState);
	EXPECT_THROW(order->pay(), OrderNotAtRightState);

	order->priceRange();
	EXPECT_THROW(order->transaction(), OrderNotAtRightState);
	OrderEvaluate evaluate;
	EXPECT_THROW(order->setEvaluate(evaluate), OrderNotAtRightState);

	EXPECT_THROW(order->rejectDate(), OrderNotAtRightState);
	order->createDate();
	EXPECT_THROW(order->receiveDate(), OrderNotAtRightState);
	EXPECT_THROW(order->startRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->finishDate(), OrderNotAtRightState);

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::unreceivedState, order->currentState());
	EXPECT_TRUE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
}

TEST_F(OrderTest, rejectTest)
{
	order->reject();

	EXPECT_THROW(order->reject(), OrderNotAtRightState);
	EXPECT_THROW(order->receivedBy(merchant), OrderNotAtRightState);
	EXPECT_THROW(order->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepair(1), OrderNotAtRightState);
	EXPECT_THROW(order->pay(), OrderNotAtRightState);

	order->priceRange();
	EXPECT_THROW(order->transaction(), OrderNotAtRightState);
	OrderEvaluate evaluate;
	EXPECT_THROW(order->setEvaluate(evaluate), OrderNotAtRightState);
	EXPECT_THROW(order->evaluate(), OrderNotAtRightState);

	order->rejectDate();
	order->createDate();
	EXPECT_THROW(order->receiveDate(), OrderNotAtRightState);
	EXPECT_THROW(order->startRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->finishDate(), OrderNotAtRightState);

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::rejectState, order->currentState());
	EXPECT_FALSE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
}

TEST_F(OrderTest, receiveTest)
{
	order->receivedBy(merchant);

	EXPECT_THROW(order->receivedBy(merchant), OrderNotAtRightState);
	EXPECT_THROW(order->reject(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepair(1), OrderNotAtRightState);
	EXPECT_THROW(order->pay(), OrderNotAtRightState);

	order->priceRange();
	EXPECT_THROW(order->transaction(), OrderNotAtRightState);
	OrderEvaluate evaluate;
	EXPECT_THROW(order->setEvaluate(evaluate), OrderNotAtRightState);

	EXPECT_THROW(order->rejectDate(), OrderNotAtRightState);
	order->createDate();
	order->receiveDate();
	EXPECT_THROW(order->startRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->finishDate(), OrderNotAtRightState);

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::receivedState, order->currentState());
	EXPECT_FALSE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
	EXPECT_EQ(merchant->id(), order->acceptorId());
}

TEST_F(OrderTest, startRepairTest)
{
	order->receivedBy(merchant);
	order->startRepair();

	EXPECT_THROW(order->reject(), OrderNotAtRightState);
	EXPECT_THROW(order->receivedBy(merchant), OrderNotAtRightState);
	EXPECT_THROW(order->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(order->pay(), OrderNotAtRightState);

	order->priceRange();
	EXPECT_THROW(order->transaction(), OrderNotAtRightState);
	OrderEvaluate evaluate;
	EXPECT_THROW(order->setEvaluate(evaluate), OrderNotAtRightState);
	EXPECT_THROW(order->evaluate(), OrderNotAtRightState);

	EXPECT_THROW(order->rejectDate(), OrderNotAtRightState);
	order->createDate();
	order->receiveDate();
	order->startRepairDate();
	EXPECT_THROW(order->endRepairDate(), OrderNotAtRightState);
	EXPECT_THROW(order->finishDate(), OrderNotAtRightState);

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::startRepairState, order->currentState());
	EXPECT_FALSE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
	EXPECT_EQ(merchant->id(), order->acceptorId());
}

TEST_F(OrderTest, endRepairTest)
{
	order->receivedBy(merchant);
	order->startRepair();
	order->endRepair(1);

	EXPECT_THROW(order->reject(), OrderNotAtRightState);
	EXPECT_THROW(order->receivedBy(merchant), OrderNotAtRightState);
	EXPECT_THROW(order->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepair(2), OrderNotAtRightState);

	order->priceRange();
	EXPECT_EQ(1, order->transaction());
	OrderEvaluate evaluate;
	EXPECT_THROW(order->setEvaluate(evaluate), OrderNotAtRightState);
	EXPECT_THROW(order->evaluate(), OrderNotAtRightState);

	EXPECT_THROW(order->rejectDate(), OrderNotAtRightState);
	order->createDate();
	order->receiveDate();
	order->startRepairDate();
	order->endRepairDate();
	EXPECT_THROW(order->finishDate(), OrderNotAtRightState);

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::endRepairState, order->currentState());
	EXPECT_FALSE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
	EXPECT_EQ(merchant->id(), order->acceptorId());
}

TEST_F(OrderTest, payTest)
{
	order->receivedBy(merchant);
	order->startRepair();
	order->endRepair(1);
	order->pay();

	EXPECT_THROW(order->reject(), OrderNotAtRightState);
	EXPECT_THROW(order->receivedBy(merchant), OrderNotAtRightState);
	EXPECT_THROW(order->startRepair(), OrderNotAtRightState);
	EXPECT_THROW(order->endRepair(2), OrderNotAtRightState);
	EXPECT_THROW(order->pay(), OrderNotAtRightState);

	order->priceRange();
	EXPECT_EQ(1, order->transaction());
	OrderEvaluate evaluate;
	order->setEvaluate(evaluate);
	//EXPECT_EQ(evaluate, order->evaluate());

	EXPECT_THROW(order->rejectDate(), OrderNotAtRightState);
	order->createDate();
	order->receiveDate();
	order->startRepairDate();
	order->endRepairDate();
	order->finishDate();

	EXPECT_EQ(string("type"), order->applianceType());
	EXPECT_EQ(string("detail"), order->detail());
	EXPECT_EQ(1, order->id());
	EXPECT_EQ(OrderState::finishedState, order->currentState());
	EXPECT_FALSE(order->isNotReceived());
	EXPECT_EQ(committer->id(), order->committerId());
	EXPECT_EQ(merchant->id(), order->acceptorId());
}
#endif