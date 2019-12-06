import QuickRepair_pb2
import QuickRepair_pb2_grpc
import grpc
from rpc.RPCServer import Server


def grpc_channel(func):
	def wrapper(*args, **kwargs):
		with grpc.insecure_channel(Server().address()) as channel:
			stub = QuickRepair_pb2_grpc.QuickRepairRPCStub(channel)
			request = func(*args, **kwargs)
			method = getattr(stub, request[0])
			return method(request[1])

	return wrapper


class Call(object):

	# @staticmethod
	# def request_verification(account_type, account):
	# 	with grpc.insecure_channel(Server().address()) as channel:
	# 		stub = QuickRepair_pb2_grpc.QuickRepairRPCStub(channel)
	# 		request = QuickRepair_pb2.VerificationRequest(
	# 			type=QuickRepair_pb2.AccountType.merchant if account_type == 'merchant' else QuickRepair_pb2.AccountType.customer,
	# 			account=account
	# 		)
	# 		return stub.requestVerification(request)
	@staticmethod
	@grpc_channel
	def request_verification(account_type, account):
		request = QuickRepair_pb2.VerificationRequest(
			type=QuickRepair_pb2.AccountType.merchant if account_type == 'merchant' else QuickRepair_pb2.AccountType.customer,
			account=account
		)
		return 'requestVerification', request

	@staticmethod
	@grpc_channel
	def get_token(account_type, account, password):
		request = QuickRepair_pb2.GetTokenRequest(
			type=QuickRepair_pb2.AccountType.merchant if account_type == 'merchant' else QuickRepair_pb2.AccountType.customer,
			account=account,
			password=password
		)
		return 'getToken', request

	@staticmethod
	@grpc_channel
	def get_merchant_list(longitude, latitude):
		request = QuickRepair_pb2.GetMerchantListRequest(
			longitude=longitude, latitude=latitude
		)
		return 'getMerchantList', request

	@staticmethod
	@grpc_channel
	def get_allowed_service():
		request = QuickRepair_pb2.GetAllowedServiceRequest()
		return 'getAllowedService', request

	@staticmethod
	@grpc_channel
	def get_merchant_service(account_id):
		request = QuickRepair_pb2.GetMerchantServiceRequest(
			id=account_id
		)
		return 'getMerchantService', request

	@staticmethod
	@grpc_channel
	def update_merchant_service(account_id, distance, token, appliances):
		app = QuickRepair_pb2.UpdateMerchantServiceRequest().appliances
		l = appliances[1:len(appliances)-1].split(", ")
		for s in l:
			app.extend(s[1:len(s)-1])
		request = QuickRepair_pb2.UpdateMerchantServiceRequest(
			merchant=account_id,
			distance=distance,
			token=token,
			appliances=app
		)
		return 'updateMerchantService', request

	@staticmethod
	@grpc_channel
	def get_order_list(account_type, account_id, token):
		request = QuickRepair_pb2.GetOrderListRequest(
			token=token,
			type=QuickRepair_pb2.AccountType.merchant if account_type == 'merchant' else QuickRepair_pb2.AccountType.customer,
			account=account_id
		)
		return 'getOrderList', request

	@staticmethod
	@grpc_channel
	def get_order_detail(order_id, token):
		request = QuickRepair_pb2.GetOrderDetailRequest(
			token=token,
			order=order_id
		)
		return 'getOrderDetail', request

	@staticmethod
	@grpc_channel
	def publish_order(receiver, committer, appliance, description, token):
		request = QuickRepair_pb2.PublishOrderRequest(
			token=token,
			receiver=receiver,
			committer=committer,
			appliance=appliance,
			description=description
		)
		return 'publishOrder', request

	@staticmethod
	@grpc_channel
	def receive_order(order_id, token, account):
		request = QuickRepair_pb2.ReceiveOrderRequest(
			order=order_id,
			token=token,
			merchant=account
		)
		return 'receiveOrder', request

	@staticmethod
	@grpc_channel
	def start_repair(order_id, token, account):
		request = QuickRepair_pb2.StartRepairRequest(
			order=order_id,
			token=token,
			merchant=account
		)
		return 'startRepair', request

	@staticmethod
	@grpc_channel
	def end_repair(order_id, token, account, transaction):
		request = QuickRepair_pb2.EndRepairRequest(
			order=order_id,
			token=token,
			merchant=account,
			transaction=transaction
		)
		return 'endRepair', request

	@staticmethod
	@grpc_channel
	def pay(order_id, token, account):
		request = QuickRepair_pb2.PayRequest(
			order=order_id,
			token=token,
			customer=account
		)
		return 'pay', request

	@staticmethod
	@grpc_channel
	def reject(order_id, token, account):
		request = QuickRepair_pb2.RejectOrderRequest(
			order=order_id,
			token=token,
			merchant=account
		)
		return 'rejectOrder', request


if __name__ == '__main__':
	print('Server:' + Server().address())
	print('send request')
	response = Call.request_verification('merchant', '12345678901')
	print('response:', response)

	print('send request')
	response = Call.get_token('merchant', '12345678901', '12345678901')
	print('response:', response.token, response.id)

