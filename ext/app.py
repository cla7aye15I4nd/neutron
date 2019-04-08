import tornado.httpserver
import tornado.ioloop

from auth import *
from tornado.options import define, options
import config

define("port", **config.global_settings)

class MainHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.render('index.html', title = 'index', user=self.current_user)
        
def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
        (r"/login", LoginHandler),
    ], **config.app_settings)

if __name__ == "__main__":
    tornado.options.parse_command_line()
    http_server = tornado.httpserver.HTTPServer(make_app())
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()