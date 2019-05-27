import tornado.web

class BaseHandler(tornado.web.RequestHandler):
    def get_current_user(self):
        return self.get_secure_cookie("user")
    def set_current_user(self, idx):
        self.set_secure_cookie("user", str(idx), httponly=True)
    def get(self):
        self.write_error(404)
    def write_error(self, status_code, **kwargs):
        self.render('error.html', title='error', user = self.current_user,stcode=status_code)