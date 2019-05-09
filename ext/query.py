import json
import tornado.web
import tornado
import config
import os
import asyncio

from urllib.parse import urlencode
from verify import get, pack, unpack, gen
from base import BaseHandler
from forms import SearchForm

class TrainHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        yield self.render('trains.html', title='Trains', user = self.current_user)

    @tornado.gen.coroutine
    def post(self):
        
        errors = "Failed"
        form = SearchForm(self.request.arguments)

        if form.validate():
            errors = "Success"
            retval = form.errors
            #对后端操作
        else: 
            retval = form.errors

        self.set_header("Content-Type","application/json")

        retval['errors'] = errors
        yield self.write(json.dumps(retval))
