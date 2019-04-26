import time
import asyncio

import tornado.gen
import tornado.httpserver
import tornado.ioloop
from tornado.options import define, options

import config
from auth import BaseHandler, LoginHandler, LogoutHandler, RegisterHandler
from user import ProfileHandler, SettingHandler, BookingHandler
from query import TrainHandler


define("port", **config.global_settings)

class IndexHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('index.html', title = 'index', user = self.current_user)
        
def make_app():
    return tornado.web.Application([
        (r"/", IndexHandler),
        (r"/login", LoginHandler),
        (r"/logout", LogoutHandler),
        (r"/register", RegisterHandler),
        (r"/profile", ProfileHandler),
        (r"/trains", TrainHandler),
        (r'/setting', SettingHandler),
        (r'/booking', BookingHandler)
    ], **config.app_settings)

if __name__ == "__main__":
    tornado.options.parse_command_line()
    app = make_app()
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
