import json
import tornado.web
from forms import LoginForm
from base import BaseHandler

class LoginHandler(BaseHandler):
    def get(self):
        self.render('login.html', title = 'Login')
        
    def post(self):
        username = self.get_argument("username", None)

        form = LoginForm(self.request.arguments)

        errors = "Failed"
        if form.validate():
            self.set_current_user(username)
            errors = "Success"
        self.set_header("Content-Type","application/json")
        self.write(json.dumps({"errors" : errors}))
        self.finish()

class RegisterHandler(BaseHandler):
    def get(self):
        self.render('register.html', title = 'Register')

class LogoutHandler(BaseHandler):
    def get(self):
        if self.get_argument("logout", None):
            self.clear_cookie("username")
            self.redirect(self.get_argument('next', '/'))