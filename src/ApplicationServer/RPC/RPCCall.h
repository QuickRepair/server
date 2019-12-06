//
// Created by gaojian on 2019/10/30.
//

#ifndef QUICK_REPAIR_RPCCALL_H
#define QUICK_REPAIR_RPCCALL_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "QuickRepair.grpc.pb.h"
#pragma GCC diagnostic pop

enum class AccountType;

class RPCCall {
public:
	enum class CallType {
		RequestVerification,
		GetToken,
		GetMerchantList,
		GetAllowedService,
		GetMerchantService,
		UpdateMerchantService,
		GetOrderList,
		GetOrderDetail,
		PublishOrder,
		ReceiveOrder,
		StartRepair,
		EndRepair,
		Pay,
		RejectOrder
	};
	RPCCall(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag, CallType type)
			: _asyncService{asyncService}, _cq{cq}, _tag{tag}, m_type{type}
	{}
	virtual void process() = 0;
	CallType type() { return m_type; }

	static AccountType getAccountType(QuickRepair::AccountType type);

protected:
	QuickRepair::QuickRepairRPC::AsyncService *_asyncService;
	grpc::ServerCompletionQueue *_cq;
	grpc::ServerContext m_context;
	void *_tag;
	CallType m_type;
};

class RequestVerification : public RPCCall {
public:
	RequestVerification(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
		: RPCCall(asyncService, cq, tag, CallType::RequestVerification), m_responder{&m_context}
	{
		_asyncService->RequestrequestVerification(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::VerificationRequest m_request;
	QuickRepair::VerificationResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::VerificationResponse> m_responder;
};

class GetToken : public RPCCall {
public:
	GetToken(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetToken), m_responder{&m_context}
	{
		_asyncService->RequestgetToken(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetTokenRequest m_request;
	QuickRepair::GetTokenResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetTokenResponse> m_responder;
};

class GetMerchantList : public RPCCall {
public:
	GetMerchantList(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetMerchantList), m_responder{&m_context}
	{
		_asyncService->RequestgetMerchantList(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetMerchantListRequest m_request;
	QuickRepair::GetMerchantListResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetMerchantListResponse> m_responder;
};

class GetAllowedService : public RPCCall {
public:
	GetAllowedService(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetAllowedService), m_responder{&m_context}
	{
		_asyncService->RequestgetAllowedService(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetAllowedServiceRequest m_request;
	QuickRepair::GetAllowedServiceResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetAllowedServiceResponse> m_responder;
};

class GetMerchantService : public RPCCall {
public:
	GetMerchantService(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetMerchantService), m_responder{&m_context}
	{
		_asyncService->RequestgetMerchantService(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetMerchantServiceRequest m_request;
	QuickRepair::GetMerchantServiceResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetMerchantServiceResponse> m_responder;
};

class UpdateMerchantService : public RPCCall {
public:
	UpdateMerchantService(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::UpdateMerchantService), m_responder{&m_context}
	{
		_asyncService->RequestupdateMerchantService(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::UpdateMerchantServiceRequest m_request;
	QuickRepair::UpdateMerchantServiceResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::UpdateMerchantServiceResponse> m_responder;
};

class GetOrderList : public RPCCall {
public:
	GetOrderList(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetOrderList), m_responder{&m_context}
	{
		_asyncService->RequestgetOrderList(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetOrderListRequest m_request;
	QuickRepair::GetOrderListResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetOrderListResponse> m_responder;
};

class GetOrderDetail : public RPCCall {
public:
	GetOrderDetail(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::GetOrderDetail), m_responder{&m_context}
	{
		_asyncService->RequestgetOrderDetail(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::GetOrderDetailRequest m_request;
	QuickRepair::GetOrderDetailResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::GetOrderDetailResponse> m_responder;
};

class PublishOrder : public RPCCall {
public:
	PublishOrder(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::PublishOrder), m_responder{&m_context}
	{
		_asyncService->RequestpublishOrder(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::PublishOrderRequest m_request;
	QuickRepair::PublishOrderResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::PublishOrderResponse> m_responder;
};

class ReceiveOrder : public RPCCall {
public:
	ReceiveOrder(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::ReceiveOrder), m_responder{&m_context}
	{
		_asyncService->RequestreceiveOrder(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::ReceiveOrderRequest m_request;
	QuickRepair::ReceiveOrderResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::ReceiveOrderResponse> m_responder;
};

class StartRepair : public RPCCall {
public:
	StartRepair(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::StartRepair), m_responder{&m_context}
	{
		_asyncService->RequeststartRepair(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::StartRepairRequest m_request;
	QuickRepair::StartRepairResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::StartRepairResponse> m_responder;
};

class EndRepair : public RPCCall {
public:
	EndRepair(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::EndRepair), m_responder{&m_context}
	{
		_asyncService->RequestendRepair(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::EndRepairRequest m_request;
	QuickRepair::EndRepairResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::EndRepairResponse> m_responder;
};

class Pay : public RPCCall {
public:
	Pay(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::Pay), m_responder{&m_context}
	{
		_asyncService->Requestpay(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::PayRequest m_request;
	QuickRepair::PayResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::PayResponse> m_responder;
};

class RejectOrder : public RPCCall {
public:
	RejectOrder(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue *cq, void *tag)
			: RPCCall(asyncService, cq, tag, CallType::RejectOrder), m_responder{&m_context}
	{
		_asyncService->RequestrejectOrder(&m_context, &m_request, &m_responder, _cq, _cq, tag);
	}
	void process() override;
private:
	QuickRepair::RejectOrderRequest m_request;
	QuickRepair::RejectOrderResponse m_response;
	grpc::ServerAsyncResponseWriter<QuickRepair::RejectOrderResponse> m_responder;
};

#endif //QUICK_REPAIR_RPCCALL_H
