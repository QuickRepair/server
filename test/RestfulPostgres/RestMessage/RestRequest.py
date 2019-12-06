#!/usr/bin/python
from Database import Postgres
from RestMessage.RestResponse import Reply
from flask_restful import Resource


class Password(Resource):

	def get(self, account):
		db = Postgres.Connection()
		result = db.get_password(account=account)
		return Reply.password(result), 200

	@staticmethod
	def binding(api):
		api.add_resource(Password, '/password/<account>')
