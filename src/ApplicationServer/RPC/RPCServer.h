//
// Created by gaojian on 2019/10/29.
//

#ifndef QUICK_REPAIR_RPCSERVER_H
#define QUICK_REPAIR_RPCSERVER_H

#include <grpcpp/grpcpp.h>
#include <unordered_map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "QuickRepair.grpc.pb.h"
#pragma GCC diagnostic pop

class RPCServer final {
public:
	RPCServer(std::string address);
	RPCServer(std::string ip, unsigned port);
	~RPCServer();

	void run();

private:
	std::unique_ptr<grpc::ServerCompletionQueue> _cq;
	QuickRepair::QuickRepairRPC::AsyncService m_asyncService;
	std::unique_ptr<grpc::Server> _server;
	grpc::ServerBuilder m_builder;
};

class RPCCall;

class ServiceUniqueTag {
public:
	ServiceUniqueTag();

	void initServices(QuickRepair::QuickRepairRPC::AsyncService *asyncService, grpc::ServerCompletionQueue* cq);
	void processService(void *tag);

private:
	void *nextAvailableTag();

	void registerAll();
	void registerService(RPCCall *call);
	template <typename CallType>
	void registerService()
	{
		void *tag = nextAvailableTag();
		m_services[tag] = Service{std::make_unique<CallType>(_asyncService, _cq, tag)};
	}

	// TODO: overflow may generate tag that's not unique
	unsigned long m_uniqueTag;
	QuickRepair::QuickRepairRPC::AsyncService *_asyncService;
	grpc::ServerCompletionQueue* _cq;

	typedef struct {
		std::unique_ptr<RPCCall> call;
		bool destroyNow = false;
	} Service;
	std::unordered_map<void *, Service> m_services;
};



#endif //QUICK_REPAIR_RPCSERVER_H
