import psycopg2


class Postgres(object):

    _conn = psycopg2.connect(database="postgres", user="postgres", password="123456", host="localhost", port="5432")

    def __init__(self):
        self._cur = Postgres._conn.cursor()

    def get_password(self, account):
        self._cur.execute('SELECT password from merchant_account where account_number=' + account + ';')
        rows = self._cur.fetchall()
        return rows[0][0]
