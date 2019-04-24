import json
import tornado.web
from forms import LoginForm, RegisterForm
from database import UserSystem
from base import BaseHandler


class AccountHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.render('account.html', title = 'Login', user = self.current_user)

class LoginHandler(BaseHandler):
    def get(self):
        self.render('login.html', title = 'Login', user = self.current_user)
        
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
        self.render('register.html', title = 'Register', user = self.current_user)

    def post(self):
        form = RegisterForm(self.request.arguments)
        errors = "Failed"
        if form.validate():
            UserSystem.register(self.request.arguments['username'][0], 
                                self.request.arguments['password'][0],
                                self.request.arguments['email'][0],
                                self.request.arguments['phone'][0])
            errors = "Success"
        self.set_header("Content-Type","application/json")

        retval = form.errors
        retval['errors'] = errors
        self.write(json.dumps(retval))
        self.finish()

class LogoutHandler(BaseHandler):
    def get(self):
        self.clear_all_cookies()
        self.redirect(self.get_argument('next', '/'))