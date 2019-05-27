import time
import asyncio

import tornado.gen
import tornado.httpserver
import tornado.ioloop
from tornado.options import define, options

import config
import admin
from auth import BaseHandler, LoginHandler, LogoutHandler, RegisterHandler, VerifyHandler
from user import ProfileHandler, SettingHandler, AvatarHandler, UploadAvatarHandler
from query import TrainHandler, TicketHandler, BookingHandler
from manage import ManageHandler
from link import link_start

define("port", **config.global_settings)

link_start()

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
        (r'/booking', BookingHandler),
        (r'/avatar', AvatarHandler),
        (r'/upload_avatar', UploadAvatarHandler),
        (r'/verify_code', VerifyHandler),
<<<<<<< HEAD
        (r'/buyticket', TicketHandler),
        (r'/manage', ManageHandler),
        (r".*", BaseHandler)

    ], **config.app_settings)
=======
    ] + admin.route, **config.app_settings)
>>>>>>> master

if __name__ == "__main__":
    tornado.options.parse_command_line()
    app = make_app()
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
