import QuickRepair_pb2
import QuickRepair_pb2_grpc
import grpc
from concurrent import futures


class Server(object):

	def __init__(self):
		self._ip = '127.0.0.1'
		self._port = 45211

	def address(self):
		return self._ip + ':' + str(self._port)


class QuickRepair(QuickRepair_pb2_grpc.QuickRepairRPCServicer):

	def requestVerification(self, request, context):
		print(
			'merchant' if request.type == QuickRepair_pb2.AccountType.merchant else 'customer',
			request.account
		)
		return QuickRepair_pb2.VerificationResponse()

	def getToken(self, request, context):
		print(
			'merchant' if request.type == QuickRepair_pb2.AccountType.merchant else 'customer',
			request.account,
			request.password
		)
		return QuickRepair_pb2.GetTokenResponse(
			token=12345678901,
			id=12345678901
		)


if __name__ == '__main__':
	server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
	QuickRepair_pb2_grpc.add_QuickRepairRPCServicer_to_server(QuickRepair(), server)
	server.add_insecure_port(Server().address())
	server.start()
	print('waiting')
	server.wait_for_termination()
	print('done')
