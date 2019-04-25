import tornado.web
from base import BaseHandler
from database import UserSystem

def formatUserMessage(userBox):
    return userBox

class ProfileHandler(BaseHandler):
    #@tornado.web.authenticated
    def get(self, username):
        if username.isalnum() and UserSystem.isExist({'username': username}):
            self.render('profile.html', title = "{} 's prfile".format(username), 
            user = self.current_user, qUser = username, info = formatUserMessage(UserSystem.queryRaw({'username': username}).fetchone()))
        else:
            self.render('warning.html', title = "Waring", user = self.current_user, message = "No such User !")