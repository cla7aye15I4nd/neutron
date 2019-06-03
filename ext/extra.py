import json
import tornado.web
import tornado
import config
import os
import asyncio

from urllib.parse import urlencode
from verify import get, pack, unpack, gen
from base import BaseHandler

class AboutHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('about.html', title = 'about', user = self.current_user)

class HelpHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('help.html', title = 'help', user = self.current_user)
