class Account(object):
    def __init__(self):
        self._account = ''
        self._password = ''
        self._id = -1
        self._name = ''
        self._orders = {}

    def account(self):
        return self._account

    def password(self):
        return self._password

    def id(self):
        return self._id

    def name(self):
        return self._name

    def set_account(self, account):
        self._account = account

    def set_password(self, password):
        self._password = password

    def set_id(self, account_id):
        self._id = account_id

    def set_name(self, name):
        self._name = name


class Merchant(Account):
    def __init__(self):
        super(Merchant, self).__init__()
        self._service = MerchantService(self)

    def service(self):
        return self._service

    def set_service(self, service):
        self._service = service


class Customer(Account):
    def __init__(self):
        super(Customer, self).__init__()


class MerchantService(object):
    def __init__(self, merchant):
        self._max_distance = -1
        self._appliances = []
        self._merchant = merchant

    def distance(self):
        return self._max_distance

    def appliance(self):
        return self._appliances

    def set_distance(self, distance):
        self._max_distance = distance

    def set_appliance(self, appliance):
        self._appliances = appliance
