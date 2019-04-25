import tornado.web

class BaseHandler(tornado.web.RequestHandler):
    def get_current_user(self):
        return self.get_secure_cookie("user")
    def set_current_user(self, idx):
        self.set_secure_cookie("user", str(idx), httponly=True)