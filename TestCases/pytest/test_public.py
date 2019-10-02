import json
import unittest

from TestCases.pytest.Connect.Connection import Connection
from TestCases.pytest.Entities.Coordinate import Coordinate


class PublicRestApi(unittest.TestCase):
    def __init__(self):
        super(PublicRestApi, self).__init__()
        self._merchant = {}

    def testGetMerchantList(self):
        coordinate = Coordinate()
        res = Connection.public_get_merchant_list(coordinate.longitude(), coordinate.latitude())
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('merchants' in json_data)
        merchants = json_data['merchants']
        self.assertTrue(type(merchants) == list)
        for m in merchants:
            self.assertTrue(type(m) == dict)
            self.assertTrue('id' in m)
            self.assertTrue('name' in m)
            self.assertTrue(type(m['id']) == int)
            self.assertTrue(type(m['name']) == str)
            self._merchant[m['id']] = m['name']
        self.assertEqual(res.status, 200)

    def testGetMerchantServices(self):
        for merchant_id in self._merchant.keys():
            res = Connection.public_get_merchant_service(merchant_id)
            json_data = json.loads(res.read().decode('utf-8'))
            self.assertTrue('max_distance' in json_data)
            self.assertTrue(type(json_data['max_distance']) == float)
            self.assertTrue('supported_appliance' in json_data)
            appliances = json_data['supported_appliance']
            self.assertTrue(type(appliances) == list)
            for appliance in appliances:
                self.assertTrue(type(appliance) == str)
            self.assertEqual(res.status, 200)

    def testGetAllowedServices(self):
        res = Connection.public_get_allowed_services()
        json_data = json.loads(res.read().decode('utf-8'))
        self.assertTrue('allowed_service' in json_data)
        services = json_data['allowed_service']
        self.assertTrue(type(services) == list)
        for appliance in services:
            self.assertTrue(type(appliance) == str)
        self.assertEqual(res.status, 200)


if __name__ == '__main__':
    unittest.main()
