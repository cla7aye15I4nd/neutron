import tornado.web

class BaseHandler(tornado.web.RequestHandler):
    def get_current_user(self):
        return self.get_secure_cookie("user")
    def set_current_user(self, username):
        self.set_secure_cookie("user", username)

class LoginHandler(BaseHandler):
    def get(self):
        self.render('login.html', title = 'Login')
        
    def post(self):
        self.set_current_user(self.get_argument("username"))
        self.redirect(self.get_argument('next', '/'))

class LogoutHandler(BaseHandler):
    def get(self):
        if self.get_argument("logout", None):
            self.clear_cookie("username")
            self.redirect(self.get_argument('next', '/'))