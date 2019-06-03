import os
import sqlite3
import hashlib
import random
from link import link_command, link_read

from emailsender import Sendmail


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
        
        code = random.randint(100000,999999)

        Sendmail(email,code)

        command2 = "register " + str(code) + " Zero 0 Zero"
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

    def checkverify(self,code):
        command = "query_profile " + self.current_user
        link_command(command)
        line = link_read()
        list = line.split()
        if(code==list[0]):
            command2 = "register " + str(code) + " Zero 1 Zero"
            link_command(command2)
            link_read()
            return True
        else:
            return False

    def isverify(self):
        command = "query_profile " + self.current_user.decode()
        link_command(command)
        line = link_read()
        list = line.split()
        if("1"==list[1]):
            return True
        else:
            return False

    def resendemail(self):
        command = "query_profile " + self.current_user.decode()
        link_command(command)
        line = link_read()
        list = line.split()
        n= int(list[1])
        if(n<-4):
            return False
        else:
            result = queryById(self.current_user.decode()).fetchone()
            
            code = random.randint(100000,999999)

            Sendmail(result[3],code)
            n-=1
            command2 = "modify_profile "+ self.current_user.decode()+ " " + str(code) + " Zero "+ str(n) +" Zero"
            link_command(command2)
            link_read()
            return True