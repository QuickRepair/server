#ifndef HAR_UNRECEIVEDORDERSTATE_H
#define HAR_UNRECEIVEDORDERSTATE_H

#include "orderstate.h"

class Order;

class OrderUnreceivedState : public OrderState, public std::enable_shared_from_this<OrderUnreceivedState> {
public:
	OrderUnreceivedState(std::weak_ptr<Order> order, OrderPriceRange &range);
	OrderUnreceivedState(std::weak_ptr<Order> order, OrderPriceRange &range, std::chrono::system_clock::time_point date);

	void receivedBy(std::weak_ptr<MerchantAccount> receiver) override;
	void startRepair() override;
	void endRepair(double transactionPrice) override;
	void orderFinished() override;

	OrderPriceRange priceRange() const override;
	double transaction() const override;
	void setEvaluate(Evaluate &evaluate) override;
	Evaluate evaluate() const override;
	std::chrono::system_clock::time_point date() const override;

private:
	std::chrono::system_clock::time_point m_stateChangeDate;
	OrderPriceRange m_range;
	std::weak_ptr<Order> m_order;
};

#endif //HAR_UNRECEIVEDORDERSTATE_H
