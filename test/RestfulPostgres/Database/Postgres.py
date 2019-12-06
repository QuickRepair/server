#!/usr/bin/python
import psycopg2


class Connection(object):

    _conn = psycopg2.connect(database="quick_repair", user="gaojian", password="gaojian", host="localhost", port="5432")

    def __init__(self):
        self._cur = Connection._conn.cursor()

    def get_password(self, account):
        self._cur.execute("SELECT password from quick_repair.account_schema.abstract_account where account_number='" + account + "';")
        rows = self._cur.fetchall()
        return rows[0][0]
