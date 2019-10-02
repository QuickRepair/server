import json
import unittest

from TestCases.pytest.Connect.Connection import Connection
from TestCases.pytest.Database.Postgres import Postgres
from TestCases.pytest.Entities.Account import Merchant
from TestCases.pytest.Entities.Account import MerchantService
from TestCases.pytest.Entities.Order import Order
from TestCases.pytest.Entities.Token import Token


class MerchantRestApi(unittest.TestCase):
    def __init__(self):
        super(MerchantRestApi, self).__init__()
        self._db = Postgres()

    def setUpClass(self):
        with open('TestCases/pytest/accounts.json') as f:
            self.account = json.load(f)['merchant']

    def testGetVerification(self):
        res = Connection.merchant_get_verification(self.account)
        self.assertEqual(res.status, 200)

    def testGetToken(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        merchant.set_password(self._db.get_password(merchant.account()))
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('token' in json_data)
        self.assertTrue('id' in json_data)
        self.assertTrue(type(json_data['token']) == str)
        self.assertTrue(type(json_data['id']) == int)
        token = Token(json_data['token'], merchant)
        merchant.set_id(json_data['id'])
        self.assertEqual(res.status, 200)

    def testGetService(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        merchant.set_password(self._db.get_password(merchant.account()))
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_data = json.loads(res.read().decode('utf-8'))
        token = Token(json_data['token'], merchant)
        merchant.set_id(json_data['id'])
        # get service
        res = Connection.merchant_get_service(merchant.id())
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('max_distance' in json_data)
        self.assertTrue('support_appliance_type' in json_data)
        self.assertTrue(type(json_data['max_distance']) == float)
        self.assertTrue(type(json_data['support_appliance_type']) == list)
        for appliance in json_data['support_appliance_type']:
            self.assertTrue(type(appliance) == str)
        self.assertEqual(res.status, 200)

    def testUpdateServiceType(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        merchant.set_password(self._db.get_password(merchant.account()))
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_data = json.loads(res.read().decode('utf-8'))
        token = Token(json_data['token'], merchant)
        merchant.set_id(json_data['id'])
        # get service
        res = Connection.merchant_get_service(merchant.id())
        json_data = json.loads(res.read().decode('utf-8'))
        service = MerchantService(merchant)
        service.set_distance(json_data['max_distance'])
        service.set_distance(json_data['support_appliance_type'])
        # update service
        service.set_distance(11)
        service.set_appliance(['a', 'b'])
        res = Connection.merchant_update_service(
            merchant.account(), token.token(), service.distance(), service.appliance()
        )
        self.assertEqual(res.status, 200)

    def testGetOrderList(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
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
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list = {}
        for o in json_data['order_list']:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # get order detail
        for o in order_list:
            res = Connection.merchant_get_order_detail(o[1], token.token())
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
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
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
            res = Connection.merchant_publish_order(token.token(), m[2], merchant.id(), 'app', 'detail')
            self.assertEqual(res.status, 200)

    def testReceiveOrder(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list = {}
        for o in json_data['order_list']:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # receive order
        for o in order_list:
            res = Connection.merchant_receive_order(o[1].id(), token.token(), merchant.id())
            self.assertEqual(res.status, 200)

    def testStartRepairOrder(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list = {}
        for o in json_data['order_list']:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # start repair
        for o in order_list:
            res = Connection.merchant_start_repair_order(o[1].id(), token.token(), merchant.id())
            self.assertEqual(res.status, 200)

    def testEndRepairOrder(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list = {}
        for o in json_data['order_list']:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # end repair
        for o in order_list:
            res = Connection.merchant_end_repair_order(o[1].id(), token.token(), merchant.id())
            self.assertEqual(res.status, 200)

    def testRejectOrder(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
        json_data = json.loads(res.read().decode('utf-8'))
        order_list = {}
        for o in json_data['order_list']:
            order = Order()
            order.set_id(o['id'])
            order.set_appliance(o['appliance_type'])
            order_list[order.id()] = order
        # end repair
        for o in order_list:
            res = Connection.merchant_reject_order(o[1].id(), token.token(), merchant.id())
            self.assertEqual(res.status, 200)

    def testPayOrder(self):
        merchant = Merchant()
        merchant.set_account(self.account)
        # generate password
        Connection.merchant_get_verification(merchant.account())
        password = self._db.get_password(account=self.account)
        merchant.set_password(password)
        # get token
        res = Connection.merchant_get_token(merchant.account(), merchant.password())
        json_token = json.loads(res.read().decode('utf-8'))
        token = Token(json_token['token'], merchant)
        # get order list
        res = Connection.merchant_get_order_list(merchant.account(), token.token())
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
            res = Connection.merchant_pay_order(o[1], token.token(), merchant.account())
            self.assertEqual(res.status, 200)


if __name__ == '__main__':
    unittest.main()
