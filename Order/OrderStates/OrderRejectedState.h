//
// Created by gaojian on 19-3-30.
//

#ifndef HARSERVER_ORDERREJECTEDSTATE_H
#define HARSERVER_ORDERREJECTEDSTATE_H

#include "OrderState.h"

class OrderRejectedState : public OrderState, public std::enable_shared_from_this<OrderRejectedState> {
public:
	OrderRejectedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState);
	OrderRejectedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::chrono::system_clock::time_point date);
	~OrderRejectedState() override = default;

	void reject() override;
	void receivedBy(std::weak_ptr<MerchantAccount> receiver) override;
	void startRepair() override;
	void endRepair(double transactionPrice) override;
	void payTheOrder() override;
	void orderFinished() override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	States atState() const override;
	std::chrono::system_clock::time_point createDate() const override;
	std::chrono::system_clock::time_point rejectDate() const override;
	std::chrono::system_clock::time_point receiveDate() const override;
	std::chrono::system_clock::time_point startRepairDate() const override;
	std::chrono::system_clock::time_point endRepairDate() const override;
	std::chrono::system_clock::time_point finishDate() const override;

private:
	std::shared_ptr<OrderState> m_lastState;
};


#endif //HARSERVER_ORDERREJECTEDSTATE_H
