import os
import sqlite3
import hashlib
from link import link_command, link_read


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

        cursor.execute(command)
        return cursor

    @classmethod
    def register(self, username, password, email, phone):
        command = "INSERT INTO user (username, password, email, phone) VALUES('{}', '{}', '{}', '{}')"
        cursor.execute(command.format(username, hashlib.sha256(password.encode('utf-8')).hexdigest(), email, phone))
        conn.commit()
        
        command2 = "register " + username + " 0 " + email + " " + phone
        link_command(command2)
        link_read()

    @classmethod
    def update(self, idx, username, email, phone):
        cursor = conn.cursor()
        command = "UPDATE user SET username = '{}', email = '{}', phone = '{}' WHERE id = {}"
        cursor.execute(command.format(username, email, phone, idx))
        conn.commit()

    @classmethod
    def changePassword(self, idx, password):
        cursor = conn.cursor()
        command = "UPDATE user SET password = '{}' WHERE id = {}"
        command.format(password, idx)
        cursor.execute(command.format(password, idx))
        conn.commit()

    @classmethod
    def queryByUsername(self, username):
        return self.queryRaw({'username': username})
    
    @classmethod
    def queryById(self, ID):
        return self.queryRaw({'ID': ID})

    @classmethod
    def isExist(self, keymap):
        return self.queryRaw(keymap).fetchone() != None

    @classmethod
    def count(self, keymap):
        return len(self.queryRaw(keymap).fetchall())
        
    @classmethod
    def checkLogin(self, username, password):
        if username.isalnum():
            key = {'username': username, 'password' : hashlib.sha256(password.encode('utf-8')).hexdigest()}
            return self.isExist(key)
        return False