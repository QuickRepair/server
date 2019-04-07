#ifndef HAR_RECEIVEDORDERSTATE_H
#define HAR_RECEIVEDORDERSTATE_H

#include "OrderState.h"

class Order;

class OrderReceivedState : public OrderState, public std::enable_shared_from_this<OrderReceivedState> {
public:
	OrderReceivedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver);
	OrderReceivedState(std::weak_ptr<Order> order, std::shared_ptr<OrderState> lastState, std::weak_ptr<MerchantAccount> receiver, std::chrono::system_clock::time_point m_stateChangeDate);
	~OrderReceivedState() override = default;

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
	std::weak_ptr<MerchantAccount> m_receiver;
	std::shared_ptr<OrderState> m_lastState;
};

#endif //HAR_RECEIVEDORDERSTATE_H
