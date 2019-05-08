import time
import asyncio

import tornado.gen
import tornado.httpserver
import tornado.ioloop
from tornado.options import define, options

import config
from auth import BaseHandler, LoginHandler, LogoutHandler, RegisterHandler, VerifyHandler
from user import ProfileHandler, SettingHandler, BookingHandler, AvatarHandler, UploadAvatarHandler
from query import TrainHandler


define("port", **config.global_settings)

class IndexHandler(BaseHandler):
    def get(self):
        self.render('index.html', title = 'index', user = self.current_user)
        
def make_app():
    return tornado.web.Application([
        (r"/", IndexHandler),
        (r"/login", LoginHandler),
        (r"/logout", LogoutHandler),
        (r"/register", RegisterHandler),
        (r"/profile", ProfileHandler),
        (r"/trains", TrainHandler),
        (r'/setting', SettingHandler),
        (r'/booking', BookingHandler),
        (r'/avatar', AvatarHandler),
        (r'/upload_avatar', UploadAvatarHandler),
        (r'/verify_code', VerifyHandler)

    ], **config.app_settings)

if __name__ == "__main__":
    tornado.options.parse_command_line()
    app = make_app()
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
