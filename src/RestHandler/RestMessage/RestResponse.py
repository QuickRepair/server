import QuickRepair_pb2
import json
from flask import make_response


class Reply(object):

	@staticmethod
	def request_verification(response):
		return make_response("", 200)

	@staticmethod
	def get_token(response):
		data = {
			'id': response.id
		}
		resp = make_response(json.dumps(data), 200)
		resp.headers.extend({'token': response.token})
		return resp

	@staticmethod
	def get_merchant_list(response):
		merchants = []
		for m in response.merchantsList:
			merchants.append(m.id)
		data = {
			'merchants': merchants
		}
		return make_response(json.dumps(data), 200)

	@staticmethod
	def get_allowed_service(response):
		appliances = []
		for a in response.appliances:
			appliances.append(a)
		data = {
			'allowed_services': appliances
		}
		return make_response(json.dumps(data), 200)

	@staticmethod
	def get_merchant_service(response):
		appliances = []
		for a in response.appliances:
			appliances.append(a)
		data = {
			'max_distance': response.max_distance,
			'supported_appliance': appliances
		}
		return make_response(json.dumps(data), 200)

	@staticmethod
	def update_merchant_service(response):
		return make_response("", 200)

	@staticmethod
	def get_order_list(response):
		orders = []
		for o in response.orders:
			order = {
				'create_date': o.create_date.ToJsonString(),
				'appliance': o.appliance,
				'id': o.id
			}
			orders.append(order)
		data = {
			'order_list': orders
		}
		return make_response(json.dumps(data), 200)

	@staticmethod
	def get_order_detail(response):
		data = {
			'id': response.id,
			'appliance': response.appliance,
			'description': response.description,
			'create_date': response.create_date.ToJsonString(),
			'receive_date': response.receive_date.ToJsonString(),
			'start_repair_date': response.start_repair_date.ToJsonString(),
			'end_repair_date': response.end_repair_date.ToJsonString(),
			'finish_date': response.finish_date.ToJsonString(),
			'reject_date': response.reject_date.ToJsonString()
		}
		if response.state == QuickRepair_pb2.OrderState.reject:
			data['state'] = 'reject'
		elif response.state == QuickRepair_pb2.OrderState.unreceived:
			data['state'] = 'unreceived'
		elif response.state == QuickRepair_pb2.OrderState.received:
			data['state'] = 'received'
		elif response.state == QuickRepair_pb2.OrderState.repairing:
			data['state'] = 'repairing'
		elif response.state == QuickRepair_pb2.OrderState.done_repair:
			data['state'] = 'done'
		elif response.state == QuickRepair_pb2.OrderState.finished:
			data['state'] = 'finished'
		return make_response(json.dumps(data), 200)

	@staticmethod
	def publish_order(response):
		return make_response("", 200)

	@staticmethod
	def receive_order(response):
		return make_response("", 200)

	@staticmethod
	def start_repair(response):
		return make_response("", 200)

	@staticmethod
	def end_repair(response):
		return make_response("", 200)

	@staticmethod
	def pay(response):
		return make_response("", 200)

	@staticmethod
	def reject(response):
		return make_response("", 200)
