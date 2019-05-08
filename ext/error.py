import json
import tornado.web
import tornado
import config
import os
import asyncio

from base import BaseHandler

class ErrorHandler(BaseHandler):
    def get(self):
        self.render('error.html', title='error', user = self.current_user)
        
   
