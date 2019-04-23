import os
import sqlite3
import hashlib
conn = sqlite3.connect(os.path.join(os.path.dirname(__file__), "neutron.db"))
cursor = conn.cursor()

class UserSystem:
    @classmethod
    def query(self, username):
        return cursor.execute("SELECT PASS FROM user WHERE NAME = '{}'".format(username)).fetchone()

    @classmethod
    def query_secure(self, username, password):
        if str(username).isalnum():
            resp = self.query(username)
            return resp != None and hashlib.sha256(password.encode('utf-8')).hexdigest() == str(resp[0])
        return False