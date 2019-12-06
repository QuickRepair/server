//
// Created by gaojian on 2019/10/30.
//

#include "RPCCall.h"
#include "ManagerProxy/AccountManagerProxy.h"
#include "ManagerProxy/TokenManagerProxy.h"
#include "ManagerProxy/OrderManagerProxy.h"
#include "ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h"
#include "Account/Account.h"
#include "Order/Order.h"

using std::to_string;				using std::list;
using std::string;					using std::make_unique;

AccountType RPCCall::getAccountType(QuickRepair::AccountType type)
{
	switch(type)
	{
		case QuickRepair::merchant:
			return AccountType::Merchant;
		case QuickRepair::customer:
			return AccountType::Customer;
		default:
			return AccountType::Unknown;
	}
}

void RequestVerification::process()
{
	AccountManagerProxy accountManagerProxy;
	AuthenticationToScreen authenticationCarrier;
	unsigned long verification = authenticationCarrier.sendVerificationCode(m_request.account());
	AccountType accountType = RPCCall::getAccountType(m_request.type());
	accountManagerProxy.createOrUpdatePasswordForAccount(accountType, m_request.account(), to_string(verification));
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetToken::process()
{
	AccountManagerProxy accountManagerProxy;
	TokenManagerProxy tokenManagerProxy;
	AccountType accountType = RPCCall::getAccountType(m_request.type());
	Account *account = accountManagerProxy.login(accountType, m_request.account(), m_request.password());
	TokenId tokenId = tokenManagerProxy.newToken(account);
	m_response.set_id(account->id());
	m_response.set_token(tokenId);
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetMerchantList::process()
{
	AccountManagerProxy accountManagerProxy;
	auto merchants = accountManagerProxy.getMerchantList();
	for(MerchantAccount *m : merchants)
	{
		QuickRepair::MerchantListInfo *info = m_response.add_merchantslist();;
		info->set_id(m->id());
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetAllowedService::process()
{
	auto services = MerchantService::getAllowedService();
	for (auto &s : services)
		m_response.add_appliances(s);
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetMerchantService::process()
{
	AccountManagerProxy accountManagerProxy;
	MerchantAccount *m = accountManagerProxy.getMerchant(m_request.id());
	MerchantService *service = m->getService();
	m_response.set_max_distance(service->maxRepairDistance());
	for (auto &s : service->supportApplianceType())
		m_response.add_appliances(s);
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void UpdateMerchantService::process()
{
	TokenManagerProxy tokenManagerProxy;
	if(tokenManagerProxy.tokenMatches(m_request.token()))
	{
		AccountManagerProxy accountManagerProxy;
		MerchantAccount *merchant = accountManagerProxy.getMerchant(m_request.merchant());
		MerchantService *service = merchant->getService();
		list<string> appliances;
		for (auto &a : m_request.appliances())
			appliances.emplace_back(a);
		service->updateService(appliances, m_request.distance());
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetOrderList::process()
{
	TokenManagerProxy tokenManagerProxy;
	if(tokenManagerProxy.tokenMatches(m_request.token()))
	{
		AccountManagerProxy accountManagerProxy;
		CustomerAccount *account;
		switch (RPCCall::getAccountType(m_request.type()))
		{
			case AccountType::Merchant:
				account = accountManagerProxy.getMerchant(m_request.account());
				break;
			case AccountType::Customer:
				account = accountManagerProxy.getCustomer(m_request.account());
				break;
			case AccountType::Unknown:
				break;
		}
		auto orderList = account->getOrderList();
	 	for (auto &o : orderList)
		{
	 		QuickRepair::OrderListInfo *info = m_response.add_orders();
	 		info->set_id(o.lock()->id());
	 		info->set_appliance(o.lock()->applianceType());
	 		// todo: does grpc take control of allocated memory?
			google::protobuf::Timestamp *createDateTimestamp = new google::protobuf::Timestamp();
			createDateTimestamp->set_seconds(std::chrono::system_clock::to_time_t(o.lock()->createDate()));
	 		info->set_allocated_create_date(createDateTimestamp);
		}
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void GetOrderDetail::process()
{
	TokenManagerProxy tokenManagerProxy;
	// todo: does grpc take control of allocated memory?
	google::protobuf::Timestamp *rejectDate = new google::protobuf::Timestamp();
	google::protobuf::Timestamp *finishDate = new google::protobuf::Timestamp();
	google::protobuf::Timestamp *endRepairDate = new google::protobuf::Timestamp();
	google::protobuf::Timestamp *startRepairDate = new google::protobuf::Timestamp();
	google::protobuf::Timestamp *receiveDate = new google::protobuf::Timestamp();
	google::protobuf::Timestamp *createDate = new google::protobuf::Timestamp();
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		OrderManagerProxy orderManagerProxy;
		auto order = orderManagerProxy.getOrder(m_request.order());
		switch(order.lock()->currentState())
		{
			case OrderState::States::rejectState:
				m_response.set_state(QuickRepair::OrderState::reject);
				rejectDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->rejectDate()));
				m_response.set_allocated_reject_date(rejectDate);
				break;
			case OrderState::States::finishedState:
				m_response.set_state(QuickRepair::OrderState::finished);
				finishDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->finishDate()));
				m_response.set_allocated_finish_date(finishDate);
			case OrderState::States::endRepairState:
				m_response.set_state(QuickRepair::OrderState::done_repair);
				endRepairDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->endRepairDate()));
				m_response.set_allocated_finish_date(endRepairDate);
			case OrderState::States::startRepairState:
				m_response.set_state(QuickRepair::OrderState::repairing);
				startRepairDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->startRepairDate()));
				m_response.set_allocated_finish_date(startRepairDate);
			case OrderState::States::receivedState:
				m_response.set_state(QuickRepair::OrderState::received);
				receiveDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->receiveDate()));
				m_response.set_allocated_finish_date(receiveDate);
			case OrderState::States::unreceivedState:
				m_response.set_state(QuickRepair::OrderState::unreceived);
				createDate->set_seconds(std::chrono::system_clock::to_time_t(order.lock()->createDate()));
				m_response.set_allocated_finish_date(createDate);
		}
		m_response.set_id(order.lock()->id());
		m_response.set_appliance(order.lock()->applianceType());
		m_response.set_description(order.lock()->detail());
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void PublishOrder::process()
{
	OrderManagerProxy orderManagerProxy;
	AccountManagerProxy accountManagerProxy;
	auto newOrder = orderManagerProxy.createOrder(m_request.committer(), m_request.receiver(), m_request.appliance(), m_request.description());
	CustomerAccount *customerAccount = accountManagerProxy.getCustomer(m_request.committer());
	MerchantAccount *merchantAccount = accountManagerProxy.getMerchant(m_request.receiver());
	customerAccount->iAmPublishAnOrder(newOrder);
	merchantAccount->orderWaitToBeAccept(newOrder);
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void ReceiveOrder::process()
{
	TokenManagerProxy tokenManagerProxy;
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		OrderManagerProxy orderManagerProxy;
		AccountManagerProxy accountManagerProxy;
		auto order = orderManagerProxy.getOrder(m_request.order());
		MerchantAccount *merchant = accountManagerProxy.getMerchant(m_request.merchant());
		order.lock()->receive(merchant);
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void StartRepair::process()
{
	TokenManagerProxy tokenManagerProxy;
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		OrderManagerProxy orderManagerProxy;
		AccountManagerProxy accountManagerProxy;
		auto order = orderManagerProxy.getOrder(m_request.order());
		MerchantAccount *merchant = accountManagerProxy.getMerchant(m_request.merchant());
		order.lock()->startRepair(merchant);
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void EndRepair::process()
{
	TokenManagerProxy tokenManagerProxy;
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		OrderManagerProxy orderManagerProxy;
		AccountManagerProxy accountManagerProxy;
		auto order = orderManagerProxy.getOrder(m_request.order());
		MerchantAccount *merchant = accountManagerProxy.getMerchant(m_request.merchant());
		order.lock()->endRepair(merchant, m_request.transaction());
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void Pay::process()
{
	TokenManagerProxy tokenManagerProxy;
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		OrderManagerProxy orderManagerProxy;
		AccountManagerProxy accountManagerProxy;
		auto order = orderManagerProxy.getOrder(m_request.order());
		CustomerAccount *customer = accountManagerProxy.getCustomer(m_request.customer());
		order.lock()->pay(customer);
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}

void RejectOrder::process()
{
	TokenManagerProxy tokenManagerProxy;
	if (tokenManagerProxy.tokenMatches(m_request.token()))
	{
		AccountManagerProxy accountManagerProxy;
		OrderManagerProxy orderManagerProxy;
		MerchantAccount *merchant = accountManagerProxy.getMerchant(m_request.merchant());
		auto order = orderManagerProxy.getOrder(m_request.order());
		order.lock()->reject(merchant);
	}
	m_responder.Finish(m_response, grpc::Status::OK, _tag);
}