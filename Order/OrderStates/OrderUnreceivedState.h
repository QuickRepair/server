#ifndef HAR_UNRECEIVEDORDERSTATE_H
#define HAR_UNRECEIVEDORDERSTATE_H

#include "OrderState.h"

class Order;

class OrderUnreceivedState : public OrderState, public std::enable_shared_from_this<OrderUnreceivedState> {
public:
	OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range);
	OrderUnreceivedState(std::weak_ptr<Order> order, AcceptableOrderPriceRange &range, std::chrono::system_clock::time_point date);
	~OrderUnreceivedState() override = default;

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
	AcceptableOrderPriceRange m_range;
	std::weak_ptr<Order> m_order;
};

#endif //HAR_UNRECEIVEDORDERSTATE_H
