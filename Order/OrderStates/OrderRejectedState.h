//
// Created by gaojian on 19-3-30.
//

#ifndef HARSERVER_ORDERREJECTEDSTATE_H
#define HARSERVER_ORDERREJECTEDSTATE_H

#include "OrderState.h"

class OrderRejectedState : public OrderState {
public:
	OrderRejectedState(std::weak_ptr<Order> order, std::unique_ptr<OrderState> &&lastState);
	~OrderRejectedState() override = default;

	std::unique_ptr<OrderState> reject() override;
	std::unique_ptr<OrderState> receive() override;
	std::unique_ptr<OrderState> startRepair() override;
	std::unique_ptr<OrderState> endRepair(double transactionPrice) override;
	std::unique_ptr<OrderState> payTheOrder() override;
	std::unique_ptr<OrderState> orderFinished() override;

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
};


#endif //HARSERVER_ORDERREJECTEDSTATE_H
