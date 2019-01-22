#include <iostream>
#include <memory>
#include "entity/order/order.h"
#include "entity/user/addressinformation.h"
#include "entity/user/merchantaccount.h"
#include "entity/order/evaluate.h"
#include "database/orderfactory.h"
#include "database/userfactory.hpp"
#include "entity/user/customeraccount.h"

using std::shared_ptr;			using std::make_shared;
using std::cout;				using std::runtime_error;

int main()
{
	try
	{
		OrderFactory orderFactory;
		UserFactory<CustomerAccount> customerFactory;
		UserFactory<MerchantAccount> merchantFactory;

		shared_ptr<MerchantAccount> merchant = merchantFactory.createUser(10, "name", "ps", "email");
		shared_ptr<MerchantAccount> dbMerchant = merchantFactory.readUser("email", "ps");
		shared_ptr<CustomerAccount> dbCustomer = customerFactory.readUser("email", "ps");

		shared_ptr<Order> newOorder = orderFactory.createOrder(dbCustomer, AddressInformation(), "detail", 123456789, OrderPriceRange(12, 54));
		shared_ptr<Order> dbOrder = orderFactory.readOrder(dbCustomer, 1234);

		dbCustomer->submitOrder(newOorder);
		dbMerchant->acceptOrder(newOorder);
		dbMerchant->startRepair(newOorder);
		dbMerchant->endRepair(newOorder, 20);
		dbCustomer->evaluateTheOrder(newOorder, Evaluate());
		cout << newOorder->priceRange().priceLow() << " " << newOorder->priceRange().priceHigh() << '\n';
		cout << newOorder->transaction() << '\n';

		dbCustomer->submitOrder(dbOrder);
		dbCustomer->cancelOrder(dbOrder);
		Evaluate eva;
		dbCustomer->evaluateTheOrder(dbOrder, eva);
		/*o->receivedBy(make_shared<MerchantAccount>());
		o->startRepair();
		o->endRepair(43);
		Evaluate eva;
		o->setEvaluate(eva);
		o->orderFinished();

		cout << dbOrder->priceRange().priceLow() << " " << dbOrder->priceRange().priceHigh() << '\n';
		cout << dbOrder->transaction() << '\n';
		Evaluate eva = dbOrder->evaluate();*/
	}
	catch(runtime_error &e)
	{
		cout << e.what();
	}

	return 0;
}
