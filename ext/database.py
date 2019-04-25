import os
import sqlite3
import hashlib

conn = sqlite3.connect(os.path.join(os.path.dirname(__file__), "neutron.db"))
cursor = conn.cursor()

class UserSystem:
    @classmethod
    def queryRaw(self, keymap):
        command = "SELECT * FROM user"
        if keymap == None:
            return cursor.execute(command)

        front = True
        command += " WHERE"
        for key, val in keymap.items():
            if not front:
                command += " AND"
            front = False
            command += " {} = '{}'".format(key, val)

        return cursor.execute(command)

    @classmethod
    def register(self, username, password, email, phone):
        command = "INSERT INTO user (username, password, email, phone) VALUES('{}', '{}', '{}', '{}')"
        cursor.execute(command.format(username, hashlib.sha256(password.encode('utf-8')).hexdigest(), email, phone))
        conn.commit()

    @classmethod
    def isExist(self, keymap):
        return self.queryRaw(keymap).fetchone() != None
        
    @classmethod
    def checkLogin(self, username, password):
        if username.isalnum():
            key = {'username': username, 'password' : hashlib.sha256(password.encode('utf-8')).hexdigest()}
            return self.isExist(key)
        return False