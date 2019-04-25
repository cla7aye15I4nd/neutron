import os.path
import tornado.options

global_settings = {
    "default" : 8000,
    "help"    : "debug version",
    "type"    : int
}

app_settings = {
    "template_path" : os.path.join(os.path.dirname(__file__), "template"),
    "static_path"   : os.path.join(os.path.dirname(__file__), "static"),
    "cookie_secret" : os.urandom(64),
    "xsrf_cookies"  : True,
    "login_url"     : "/login",
    "debug"         : True
}
