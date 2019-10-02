class Order(object):

    def __init__(self):
        self._id = 0
        self._state = ''
        self._committer = 0
        self._receiver = 0
        self._appliance = ''

    def id(self):
        return self._id

    def state(self):
        return self._state

    def committer(self):
        return self._committer

    def receiver(self):
        return self._receiver

    def appliance(self):
        return self._appliance

    def set_id(self, order_id):
        self._id = order_id

    def set_state(self, state):
        self._state = state

    def set_committer(self, committer):
        self._committer = committer

    def set_receiver(self, receiver):
        self._receiver = receiver

    def set_appliance(self, appliance):
        self._appliance = appliance