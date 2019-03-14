#ifndef HAR_STARTREPAIRORDERSTATE_H
#define HAR_STARTREPAIRORDERSTATE_H

#include "OrderState.h"

class Order;

class OrderStartRepairState : public OrderState, public std::enable_shared_from_this<OrderStartRepairState> {
public:
	OrderStartRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState);
	OrderStartRepairState(std::weak_ptr<Order> order, std::weak_ptr<OrderState> lastState, std::chrono::system_clock::time_point date);
	~OrderStartRepairState() override = default;

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
	std::chrono::system_clock::time_point m_stateChangeDate;
	std::weak_ptr<Order> m_order;
	std::weak_ptr<OrderState> m_lastState;
};

#endif //HAR_STARTREPAIRORDERSTATE_H
