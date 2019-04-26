import json
import tornado.web
import tornado

from base import BaseHandler
from forms import LoginForm, RegisterForm
from database import UserSystem


class LoginHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('login.html', title = 'Login', user = self.current_user)
        
    @tornado.gen.coroutine
    def post(self):
        username = self.get_argument("username", None)
        form = LoginForm(self.request.arguments)
        errors = "Failed"
        if form.validate():
            self.set_current_user(UserSystem.queryByUsername(username).fetchone()[0])
            errors = "Success"
        self.set_header("Content-Type","application/json")
        yield self.write(json.dumps({"errors" : errors}))

class RegisterHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('register.html', title = 'Register', user = self.current_user)

    @tornado.gen.coroutine
    def post(self):
        form = RegisterForm(self.request.arguments)
        errors = "Failed"
        if form.validate():
            UserSystem.register(self.request.arguments['username'][0].decode(), 
                                self.request.arguments['password'][0].decode(),
                                self.request.arguments['email'][0].decode(),
                                self.request.arguments['phone'][0].decode())
            errors = self.request.arguments['username'][0].decode()
        self.set_header("Content-Type","application/json")

        retval = form.errors
        retval['errors'] = errors
        yield self.write(json.dumps(retval))

class LogoutHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        self.clear_all_cookies()
        yield self.redirect(self.get_argument('next', '/'))