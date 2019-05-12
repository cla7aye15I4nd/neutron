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
            errors = "Nothing"
            retval = form.errors

            
            #对后端操作
            if(self.request.arguments['date'][0].decode() == "2019-05-12"):
                errors = "Success"
                
                retval["user"] = self.current_user

                retval["cnt"] = 1
                retval["seatcnt0"] = 1
                retval["code0"] = "Fuck"
                retval["start0"] = "06:30"
                retval["startpos0"] = "BBBBeijing"
                retval["end0"] = "08:00"
                retval["endpos0"] = "SSSHANGHAI"
                retval["time0"] = "1h30m"

                retval["seatclass0|0"] = " "#无票则为 seat-sold-out sold-out-warn
                retval["seatbutton0|0"] = "BOOK"#无票则为 SOLD OUT
                

                retval["seattype0|0"] = "MASTERSEAT"
                retval["seatmoney0|0"] = "1000.00"
                retval["seatleft0|0"] = "5 left"#无票为空
                
        else: 
            retval = form.errors

        self.set_header("Content-Type","application/json")

        retval['errors'] = errors
        yield self.write(json.dumps(retval))
