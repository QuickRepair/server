from RestMessage.RestResponse import Reply
from flask_restful import Resource, reqparse
from rpc import RPCClient


class Verification(Resource):

	def get(self, account_type, account):
		# try:
		#
		# except grpc.RpcError as rpc_error:
		# 	status = rpc_status.from_call(rpc_error)
		# 	for details in status.details:
		# 		print(details)
		# finally:
		#
		response = RPCClient.Call.request_verification(account_type, account)
		return Reply.request_verification(response)

	@staticmethod
	def binding(api):
		api.add_resource(Verification, '/verification/<account_type>/<account>')


class Token(Resource):
	def __init__(self):
		self._parser = reqparse.RequestParser()
		self._parser.add_argument('password', location='headers')

	def get(self, account_type, account):
		args = self._parser.parse_args()
		response = RPCClient.Call.get_token(account_type, account, args['password'])
		return Reply.get_token(response)

	@staticmethod
	def binding(api):
		api.add_resource(Token, '/token/<account_type>/<account>')


class MerchantList(Resource):

	def get(self, longitude, latitude):
		response = RPCClient.Call.get_merchant_list(float(longitude), float(latitude))
		return Reply.get_merchant_list(response)

	@staticmethod
	def binding(api):
		api.add_resource(MerchantList, '/merchant_list/<longitude>/<latitude>')


class Order(Resource):
	def __init__(self):
		self._parser = reqparse.RequestParser()
		self._parser.add_argument('token', location='headers')
		self._parser.add_argument('receiver', location='json')
		self._parser.add_argument('committer', location='json')
		self._parser.add_argument('appliance', location='json')
		self._parser.add_argument('description', location='json')
		self._parser.add_argument('account', location='json')
		self._parser.add_argument('state', location='json')
		self._parser.add_argument('transaction', location='json')

	def get(self, order):
		args = self._parser.parse_args()
		response = RPCClient.Call.get_order_detail(int(order), int(args['token']))
		return Reply.get_order_detail(response)

	def post(self):
		args = self._parser.parse_args()
		response = RPCClient.Call.publish_order(
			int(args['receiver']), int(args['committer']), args['appliance'], args['description'], int(args['token'])
		)
		return Reply.publish_order(response)

	def put(self, order):
		args = self._parser.parse_args()
		if args['state'] == 'receive':
			response = RPCClient.Call.receive_order(int(order), int(args['token']), int(args['account']))
			return Reply.receive_order(response)
		elif args['state'] == 'start_repair':
			response = RPCClient.Call.start_repair(int(order), int(args['token']), int(args['account']))
			return Reply.start_repair(response)
		elif args['state'] == 'end_repair':
			response = RPCClient.Call.end_repair(int(order), int(args['token']), int(args['account']), args['transaction'])
			return Reply.end_repair(response)
		elif args['state'] == 'pay':
			response = RPCClient.Call.pay(int(order), int(args['token']), int(args['account']))
			return Reply.pay(response)
		elif args['state'] == 'reject':
			response = RPCClient.Call.reject(int(order), int(args['token']), int(args['account']))
			return Reply.reject(response)

	@staticmethod
	def binding(api):
		api.add_resource(Order, '/order', '/order/<order>')


class AllowedService(Resource):

	def get(self):
		response = RPCClient.Call.get_allowed_service()
		return Reply.get_allowed_service(response)

	@staticmethod
	def binding(api):
		api.add_resource(AllowedService, '/allowed_service')


class SupportedService(Resource):
	def __init__(self):
		self._parser = reqparse.RequestParser()
		self._parser.add_argument('token', location='headers')
		self._parser.add_argument('max_distance', location='json')
		self._parser.add_argument('support_appliances', location='json')

	def get(self, account_id):
		response = RPCClient.Call.get_merchant_service(int(account_id))
		return Reply.get_merchant_service(response)

	def put(self, account_id):
		args = self._parser.parse_args()
		response = RPCClient.Call.update_merchant_service(
			int(account_id), float(args['max_distance']), int(args['token']), args['support_appliances']
		)
		return Reply.update_merchant_service(response)

	@staticmethod
	def binding(api):
		api.add_resource(SupportedService, '/supported_service/<account_id>')


class OrderList(Resource):
	def __init__(self):
		self._parser = reqparse.RequestParser()
		self._parser.add_argument('token', location='headers')

	def get(self, account_type, account):
		args = self._parser.parse_args()
		response = RPCClient.Call.get_order_list(
			account_type, int(account), int(args['token'])
		)
		return Reply.get_order_list(response)

	@staticmethod
	def binding(api):
		api.add_resource(OrderList, '/order_list/<account_type>/<account>')


if __name__ == "__main__":
	r = RPCClient.Call.request_verification('merchant', '12345678901')
	print(Reply.request_verification(r))
