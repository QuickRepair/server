class Host(object):

    def __init__(self):
        self._host_ip = '127.0.0.1'
        self._api_url = 'http://' + self._host_ip + '/api/v1/'

    def ip(self):
        return self._host_ip

    def api_url(self):
        return self._api_url
