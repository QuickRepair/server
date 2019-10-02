class Token(object):
    def __init__(self, token_id, account):
        self._token_id = token_id
        self._account = account

    def token(self):
        return self._token_id

    def account(self):
        return self._account