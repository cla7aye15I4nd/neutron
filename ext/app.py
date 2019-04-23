import tornado.httpserver
import tornado.ioloop

from auth import BaseHandler, LoginHandler, LogoutHandler, RegisterHandler
from query import TrainHandler

from tornado.options import define, options
import config

define("port", **config.global_settings)

class IndexHandler(BaseHandler):
    def get(self):
        self.render('index.html', title = 'index', user = self.get_current_user())
        
def make_app():
    return tornado.web.Application([
        (r"/", IndexHandler),
        (r"/login", LoginHandler),
        (r"/logout", LogoutHandler),
        (r"/register", RegisterHandler),
        (r"/trains", TrainHandler)
    ], **config.app_settings)

if __name__ == "__main__":
    tornado.options.parse_command_line()
    http_server = tornado.httpserver.HTTPServer(make_app())
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()