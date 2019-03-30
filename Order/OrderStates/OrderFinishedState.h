#ifndef HAR_ORDERFINISHEDSTATE_H
#define HAR_ORDERFINISHEDSTATE_H

#include "OrderState.h"

class Order;

class OrderFinishedState : public OrderState {
public:
	OrderFinishedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState);
	OrderFinishedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::chrono::system_clock::time_point date);
	~OrderFinishedState() override = default;

	void reject() override;
	void receivedBy(std::weak_ptr<MerchantAccount> receiver) override;
	void startRepair() override;
	void endRepair(double transactionPrice) override;
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

#endif //HAR_ORDERFINISHEDSTATE_H
