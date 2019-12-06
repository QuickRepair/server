#!/usr/bin/python
from RestMessage import RestRequest
from flask import Flask
from flask_restful import Api

app = Flask(__name__)

api = Api(app)
RestRequest.Password.binding(api)

app.run(port=45210)
