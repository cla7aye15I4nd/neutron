import os
import sqlite3
import hashlib
conn = sqlite3.connect(os.path.join(os.path.dirname(__file__), "neturon.db"))
cursor = conn.cursor()

class UserSystem:
    @classmethod
    def query(username):
        return cursor.execute("SELECT PASS FROM user WHERE NAME = '{}'".format(username)).fetchone()

    @classmethod
    def query_secure(username, password):
        if str(username).isalnum():
            resp = query(username)
            return resp != None and hashlib.sha256(password).hexdigest() == resp[0]
        return False