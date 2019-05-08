import tornado.web
import config
import os

class BaseAdminHandler(tornado.web.RequestHandler):
    def is_admin(self):
        if not self.get_secure_cookie("name") is None:
            return self.get_secure_cookie("name").decode() in config.admin_name
        return False
    def set_admin(self, name):
        return self.set_secure_cookie("name", name, httponly=True)

    def only_admin(self, func):
        def wrapper(*args, **kwargs):
            if self.is_admin():
                return func(*args, **kwargs)
        return wrapper

    def render_template(self, template_name, **kwargs):
        self.render(os.path.join('admin', template_name), **kwargs)

class AdminHandler(BaseAdminHandler):
    def login_page(self):
        self.render_template("login.html")

    def get(self):
        if self.is_admin():
            self.write("hello")
        else:
            self.login_page()

class AdminLoginHandler(BaseAdminHandler):
    def post(self):
        if (self.get_argument('name', None), self.get_argument('passwd', None)) in config.admin:
            self.set_admin(self.get_argument('name'))
        return self.redirect('/admin')
            
admin_route = [
    (r'/admin', AdminHandler),
    (r'/admin/login', AdminLoginHandler)
]