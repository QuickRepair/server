#ifndef HAR_ORDERFINISHEDSTATE_H
#define HAR_ORDERFINISHEDSTATE_H

#include "OrderState.h"

class Order;

class OrderFinishedState : public OrderState {
public:
	OrderFinishedState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState);
	~OrderFinishedState() override = default;

	void receivedBy(std::weak_ptr<MerchantAccount> receiver) override;
	void startRepair() override;
	void endRepair(double transactionPrice) override;
	void orderFinished() override;

	AcceptableOrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(OrderEvaluate &evaluate) override;
	OrderEvaluate evaluate() const override;
	std::chrono::system_clock::time_point date() const override;

private:
	std::weak_ptr<Order> m_order;
	std::weak_ptr<OrderState> m_lastState;
};

#endif //HAR_ORDERFINISHEDSTATE_H
