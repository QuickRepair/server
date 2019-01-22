#include "ordermanagesystem.h"
#include "../../database/orderfactory.h"
#include "../../entity/user/customeraccount.h"
#include "../../entity/user/merchantaccount.h"
#include "../../entity/order/orderpricerange.h"
#include "../../entity/user/addressinformation.h"
#include "../../network/jsonAnalyser.h"

using std::make_shared;			using std::shared_ptr;
using std::string;

OrderManageSystem OrderManageSystem::getInstance()
{
	static OrderManageSystem instance = OrderManageSystem();
	return instance;
}

std::string OrderManageSystem::shunt_message(const std::string &data)
{
	string str;
	int type=atoi(JsonAnalyser::Init().json_get(JsonAnalyser::JsonData().TYPE, data).data());
	switch(type){
		case 0:
			str="number 1";
			break;
		case 2:
			str="number 2";
			break;
		default:
			str="unknow command";
			break;
	}
	str=JsonAnalyser::Init().json_set(JsonAnalyser::JsonData().DATA, str);
	str=JsonAnalyser::Init().json_set(JsonAnalyser::JsonData().TYPE, "0");
	return str;
}

OrderManageSystem::OrderManageSystem()
{
	m_factory = make_shared<OrderFactory>();
}

void OrderManageSystem::requestUploadOrder(std::weak_ptr<CustomerAccount> &commiter, AddressInformation address, std::string detail, OrderPriceRange range)
{
	unsigned long id = 1000;//TODO set id
	shared_ptr<Order> newOrder = m_factory->createOrder(commiter, address, detail, id, range);
	commiter.lock()->submitOrder(newOrder);
}

void OrderManageSystem::requestStartRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->startRepair(order);
}

void OrderManageSystem::requestEndRepair(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order, double transaction)
{
	acceptor.lock()->endRepair(order, transaction);
}

void OrderManageSystem::requestPayOrder(std::weak_ptr<CustomerAccount> &commiter, std::weak_ptr<Order> &order)
{
	commiter.lock()->payTheOrder(order);
}

void OrderManageSystem::requestAcceptOrder(std::weak_ptr<MerchantAccount> &acceptor, std::weak_ptr<Order> &order)
{
	acceptor.lock()->acceptOrder(order);
}

std::list<std::shared_ptr<Order>> OrderManageSystem::requestQueryOrder(std::weak_ptr<User> user)
{
	return user.lock()->myOrders();
}
