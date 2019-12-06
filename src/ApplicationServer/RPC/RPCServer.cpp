//
// Created by gaojian on 2019/10/29.
//

#include "RPCServer.h"
#include "RPCCall.h"

using std::string;			using std::to_string;
using std::make_unique;

RPCServer::RPCServer(std::string address)
{
	m_builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	m_builder.RegisterService(&m_asyncService);
}

RPCServer::RPCServer(std::string ip, unsigned port)
{
	m_builder.AddListeningPort(ip + to_string(port), grpc::InsecureServerCredentials());
	m_builder.RegisterService(&m_asyncService);
}

RPCServer::~RPCServer()
{
	_server->Shutdown();
	_cq->Shutdown();
}

void RPCServer::run()
{
	_cq = m_builder.AddCompletionQueue();
	_server = m_builder.BuildAndStart();

	ServiceUniqueTag uniqueTag;
	uniqueTag.initServices(&m_asyncService, _cq.get());

	void *tag;
	bool ok;
	while(true) {
		_cq->Next(&tag, &ok);
		GPR_ASSERT(ok);
		uniqueTag.processService(tag);
	}
}

ServiceUniqueTag::ServiceUniqueTag()
	: m_uniqueTag{0}
{}

void ServiceUniqueTag::initServices(QuickRepair::QuickRepairRPC::AsyncService *asyncService,
									grpc::ServerCompletionQueue *cq)
{
	_asyncService = asyncService;
	_cq = cq;
	registerAll();
}

void ServiceUniqueTag::processService(void *tag)
{
	if(m_services[tag].destroyNow)
		m_services.erase(tag);
	else
	{
		registerService(m_services[tag].call.get());
		m_services[tag].call->process();
		m_services[tag].destroyNow = true;
	}
}

void* ServiceUniqueTag::nextAvailableTag()
{
	return (void *)++m_uniqueTag;
}

void ServiceUniqueTag::registerAll()
{
	registerService<RequestVerification>();
	registerService<GetToken>();
	registerService<GetMerchantList>();
	registerService<GetAllowedService>();
	registerService<GetMerchantService>();
	registerService<UpdateMerchantService>();
	registerService<GetOrderList>();
	registerService<GetOrderDetail>();
	registerService<PublishOrder>();
	registerService<ReceiveOrder>();
	registerService<StartRepair>();
	registerService<EndRepair>();
	registerService<Pay>();
	registerService<RejectOrder>();
}

void ServiceUniqueTag::registerService(RPCCall *call)
{
	switch(call->type())
	{
		case RPCCall::CallType::RequestVerification:	registerService<RequestVerification>();	break;
		case RPCCall::CallType::GetToken:				registerService<GetToken>();			break;
		case RPCCall::CallType::GetMerchantList:		registerService<GetMerchantList>();		break;
		case RPCCall::CallType::GetAllowedService:		registerService<GetAllowedService>();	break;
		case RPCCall::CallType::GetMerchantService:		registerService<GetMerchantService>();	break;
		case RPCCall::CallType::UpdateMerchantService:	registerService<UpdateMerchantService>();break;
		case RPCCall::CallType::GetOrderList:			registerService<GetOrderList>();		break;
		case RPCCall::CallType::GetOrderDetail:			registerService<GetOrderDetail>();		break;
		case RPCCall::CallType::PublishOrder:			registerService<PublishOrder>();		break;
		case RPCCall::CallType::ReceiveOrder:			registerService<ReceiveOrder>();		break;
		case RPCCall::CallType::StartRepair:			registerService<StartRepair>();			break;
		case RPCCall::CallType::EndRepair:				registerService<EndRepair>();			break;
		case RPCCall::CallType::Pay:					registerService<Pay>();					break;
		case RPCCall::CallType::RejectOrder:			registerService<RejectOrder>();			break;
	}
}