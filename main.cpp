#include <iostream>
#include <memory>
#include "Managers/AccountManager.h"
#include "Managers/OrderManager.h"
#include "RestServer/RestHandler.h"
#include "Managers/AuthenticationCarrier/AuthenticationToScreen.h"
#include "Configuration/Configure.h"
#include "Factories/Database/DatabaseConnection.h"

using std::shared_ptr;			using std::make_shared;
using std::cout;				using std::runtime_error;

int main()
{
	try
	{
		Configure configure("./har.json");
		DatabaseConnection::getInstance().conenct(
				configure.databaseIp(), configure.databaseName(),
				configure.databaseUserName(), configure.databasePassword(),
				configure.databasePort()
				);
		AccountManager::getInstance().registerAuthenticationCarrier(make_shared<AuthenticationToScreen>());
		RestHandler handler(configure.listenOn());
		while (true)
			continue;
		/*OrderFactory orderFactory;
		AccountFactory<CustomerAccount> customerFactory;
		AccountFactory<MerchantAccount> merchantFactory;

		shared_ptr<CustomerAccount> committer = customerFactory.createOrUpdateUser("a", "b");

		//send verification code
		std::string verificationDeviceIdentification;
		AccountManager::getInstance().requestForVerificationCode(verificationDeviceIdentification);

		//check password
		std::string userName;
		std::string password;
		AccountManager::getInstance().accountAuthentication<MerchantAccount>(account, password);

		//get merchant list
		for(auto &merchants : AccountManager::getInstance().getMerchantList())
			merchants.lock()->myConciseInfo();

		//publish Order
		ContactInformation contact;
		std::string detail;
		AcceptableOrderPriceRange range;
		OrderManager::getInstance().publishOrder(committer, contact, detail, range);*/

		/*shared_ptr<MerchantAccount> merchant = merchantFactory.createOrUpdateUser(10, "name", "ps", "email");
		shared_ptr<MerchantAccount> dbMerchant = merchantFactory.readUser("email", "ps");
		shared_ptr<CustomerAccount> dbCustomer = customerFactory.readUser("email", "ps");

		shared_ptr<Order> newOorder = orderFactory.createOrder(dbCustomer, ContactInformation(), "detail", 123456789, AcceptableOrderPriceRange(12, 54));
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
		dbCustomer->evaluateTheOrder(dbOrder, eva);*/
		/*o->receivedBy(make_shared<MerchantAccount>());
		o->startRepair();
		o->endRepair(43);
		OrderEvaluate eva;
		o->setEvaluate(eva);
		o->orderFinished();

		cout << dbOrder->priceRange().lowerPrice() << " " << dbOrder->priceRange().upperPrice() << '\n';
		cout << dbOrder->transaction() << '\n';
		OrderEvaluate eva = dbOrder->evaluate();*/
	}
	catch(runtime_error &e)
	{
		cout << e.what();
	}

	return 0;
}
