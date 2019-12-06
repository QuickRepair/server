from RestMessage import RestRequest
from flask import Flask
from flask_restful import Api

app = Flask(__name__)

api = Api(app)
RestRequest.Verification.binding(api)
RestRequest.Token.binding(api)
RestRequest.MerchantList.binding(api)
RestRequest.Order.binding(api)
RestRequest.AllowedService.binding(api)
RestRequest.SupportedService.binding(api)
RestRequest.OrderList.binding(api)

app.run(host='0.0.0.0', port=45212)
