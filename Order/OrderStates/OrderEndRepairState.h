#ifndef HAR_ENDREPAIRORDERSTATE_H
#define HAR_ENDREPAIRORDERSTATE_H

#include "OrderState.h"

class Order;

class OrderEndRepairState : public OrderState, public std::enable_shared_from_this<OrderEndRepairState> {
public:
	OrderEndRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, double transactionPrice);
	OrderEndRepairState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, double transactionPrice, std::chrono::system_clock::time_point date);
	~OrderEndRepairState() override = default;

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
	double m_transaction;
};

#endif //HAR_ENDREPAIRORDERSTATE_H
