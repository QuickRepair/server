
#ifndef HAR_ORDERFACTORY_H
#define HAR_ORDERFACTORY_H

#include <memory>

class Order;
class OrderStateAbstractFactory;
class OrderState;
class CustomerAccount;
class ContactInformation;
class AcceptableOrderPriceRange;

class OrderFactory {
public:
	std::shared_ptr<Order> readOrder(std::weak_ptr<CustomerAccount> commiter, unsigned long orderId);
	std::shared_ptr<Order> createOrder(std::weak_ptr<CustomerAccount> commiter, ContactInformation address, std::string detail, AcceptableOrderPriceRange range);

private:
	std::shared_ptr<OrderState> getStates(std::shared_ptr<Order> &order, unsigned long stateId, unsigned call = 0);
};

#endif //HAR_ORDERFACTORY_H
