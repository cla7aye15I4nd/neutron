import tornado.web
from forms import LoginForm

class BaseHandler(tornado.web.RequestHandler):
    def get_current_user(self):
        return self.get_secure_cookie("user")
    def set_current_user(self, username):
        self.set_secure_cookie("user", username)
    
class LoginHandler(BaseHandler):
    def get(self):
        self.render('login.html', title = 'Login')
        
    def post(self):
        username = self.get_argument("username", None)

        form = LoginForm(self.request.arguments)
        form.validate()

        print (form.errors)
        if form.errors == {}:
            self.set_current_user(username)
            self.redirect(self.get_argument('next', '/'))

class LogoutHandler(BaseHandler):
    def get(self):
        if self.get_argument("logout", None):
            self.clear_cookie("username")
            self.redirect(self.get_argument('next', '/'))