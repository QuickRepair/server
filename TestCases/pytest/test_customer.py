import json
import unittest

from TestCases.pytest.Connect.Connection import Connection
from TestCases.pytest.Database.Postgres import Postgres
from TestCases.pytest.Entities.Account import Customer
from TestCases.pytest.Entities.Account import Merchant
from TestCases.pytest.Entities.Order import Order
from TestCases.pytest.Entities.Token import Token


class CustomerRestApi(unittest.TestCase):
    def __init__(self):
        super(CustomerRestApi, self).__init__()
        self._db = Postgres()

    def setUp(self):
        with open('TestCases/pytest/accounts.json') as f:
            self.account = json.load(f)['customer']

    def testGetVerification(self):
        res = Connection.customer_get_verification(self.account)
        self.assertEqual(res.status, 200)

    def testGetToken(self):
        customer = Customer()
        password = self._db.get_password(account=self.account)
        customer.set_account(self.account)
        customer.set_password(password)
        res = Connection.customer_get_token(customer.account(), customer.password())
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('token' in json_data)
        self.assertTrue('id' in json_data)
        self.assertTrue(type(json_data['token']) == str)
        self.assertTrue(type(json_data['id']) == int)
        token = Token(json_data['token'], customer)
        customer.set_id(json_data['id'])
        self.assertEqual(res.status, 200)

    def testGetOrderList(self):
        customer = Customer()
        customer.set_account(self.account)
        # generate password
        Connection.customer_get_verification(customer.account())
        password = self._db.get_password(account=self.account)
        customer.set_password(password)
        # get token
        res = Connection.customer_get_token(customer.account(), customer.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], customer)
        # get order list
        res = Connection.customer_get_order_list(customer.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('order_list' in json_data)
        self.assertTrue(type(json_data['token']) == list)
        order_list = json_data['token']
        for o in order_list:
            self.assertTrue('create_date' in o)
            self.assertTrue('appliance_type' in o)
            self.assertTrue('id' in o)
            self.assertTrue(type(o['create_date']) == str)
            self.assertTrue(type(o['appliance_type']) == str)
            self.assertTrue(type(o['id']) == int)
        self.assertEqual(res.status, 200)

    def testGetOrderDetail(self):
        customer = Customer()
        customer.set_account(self.account)
        # generate password
        Connection.customer_get_verification(customer.account())
        password = self._db.get_password(account=self.account)
        customer.set_password(password)
        # get token
        res = Connection.customer_get_token(customer.account(), customer.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], customer)
        # get order list
        res = Connection.customer_get_order_list(customer.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list_data = json_data['token']
        order_list = {}
        for o in order_list_data:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # get order detail
        for o in order_list:
            res = Connection.customer_get_order_detail(o[1], token.token())
            detail_data = json.loads(res.read().decode('utf-8'))
            self.assertTrue('id' in detail_data)
            self.assertTrue('appliance_type' in detail_data)
            self.assertTrue('detail' in detail_data)
            self.assertTrue('state' in detail_data)
            self.assertTrue('create_date' in detail_data)
            self.assertTrue('receive_date' in detail_data)
            self.assertTrue('start_repair_date' in detail_data)
            self.assertTrue('end_repair_date' in detail_data)
            self.assertTrue('finish_date' in detail_data)
            self.assertTrue('reject_date' in detail_data)
            self.assertTrue(type(detail_data['id']) == int)
            self.assertTrue(type(detail_data['appliance_type']) == str)
            self.assertTrue(type(detail_data['detail']) == str)
            self.assertTrue(type(detail_data['state']) == str)
            self.assertTrue(type(detail_data['create_date']) == str)
            self.assertTrue(type(detail_data['receive_date']) == str)
            self.assertTrue(type(detail_data['start_repair_date']) == str)
            self.assertTrue(type(detail_data['end_repair_date']) == str)
            self.assertTrue(type(detail_data['finish_date']) == str)
            self.assertTrue(type(detail_data['reject_date']) == str)
            self.assertEqual(res.status, 200)

    def testPublishOrder(self):
        customer = Customer()
        customer.set_account(self.account)
        # generate password
        Connection.customer_get_verification(customer.account())
        password = self._db.get_password(account=self.account)
        customer.set_password(password)
        # get token
        res = Connection.customer_get_token(customer.account(), customer.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], customer)
        # get merchant list
        res = Connection.public_get_merchant_list(0, 0)
        merchant_json = json.loads(res.read().decode('utf-8'))
        merchant_list = {}
        for m in merchant_json:
            merchant = Merchant()
            merchant.set_id(m['id'])
            merchant.set_name(m['name'])
            merchant_list[merchant.id()] = merchant
        # publish order
        for m in merchant_list:
            res = Connection.customer_publish_order(token.token(), m[2], customer.id(), 'app', 'detail')
            self.assertEqual(res.status, 200)

    def testPayOrder(self):
        customer = Customer()
        customer.set_account(self.account)
        # generate password
        Connection.customer_get_verification(customer.account())
        password = self._db.get_password(account=self.account)
        customer.set_password(password)
        # get token
        res = Connection.customer_get_token(customer.account(), customer.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], customer)
        # get order list
        res = Connection.customer_get_order_list(customer.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list_data = json_data['token']
        order_list = {}
        for o in order_list_data:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # pay the order
        for o in order_list:
            res = Connection.customer_pay_order(o[1], token.token(), customer.account())
            self.assertEqual(res.status, 200)


if __name__ == '__main__':
    unittest.main()
