#!/usr/bin/python
import json


class Reply(object):

	@staticmethod
	def password(result):
		data = {'password': result}
		return json.dumps(data)
