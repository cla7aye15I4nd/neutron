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
from database import UserSystem
from link import link_command, link_read

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

            require = self.request.arguments['froms'][0].decode() + ' ' + self.request.arguments['arrival'][0].decode() + ' ' + self.request.arguments['date'][0].decode()
            if(self.request.arguments['type'][0].decode() == ''):
                require+=" 0"
            else:
                require+=" 1 "
                require+=self.request.arguments['type'][0].decode()
            command = require

            if(self.request.arguments['available'][0].decode() == "true" and self.request.arguments['change'][0].decode() == "true"):
                command = "query_ticket_transfer_available " + require
            elif(self.request.arguments['available'][0].decode() == "true"):
                command = "query_ticket_available " + require
            elif(self.request.arguments['change'][0].decode() == "true"):
                command = "query_ticket_transfer " + require
            else:
                command = "query_ticket_all " + require
            
            print(command)

            link_command(command)

            n = int(link_read())
            
            if(n):
                errors = "Success"
                #retval["user"] = self.current_user.decode()
                retval["cnt"] = n
                for i in range(0,n):
                    ii = str(i)
                    line = link_read()
                    list = line.split()
                    m = int(link_read())
                    retval["seatcnt" + ii] = m
                    retval["code" + ii] = list[0]
                    retval["startpos" + ii] = list[1]
                    retval["start" + ii] = list[2]
                    retval["endpos" + ii] = list[3]
                    retval["end" + ii] = list[4]#多一天则加 <span class='day-plus'>+1d</span>
                    if(list[5] == '1'): retval["end" + ii]+="<span class='day-plus'>+1d</span>"
                    retval["time" + ii] = list[6]
                    for j in range(0,m):
                        jj = str(j)
                        line = link_read()
                        list = line.split()
                        retval["seattype" + ii + "|" + jj] = list[0]
                        if(list[1] == '0'):
                            retval["seatclass" + ii + "|" + jj] = "seat-sold-out sold-out-warn"
                            retval["seatbutton" + ii + "|" + jj] = "SOLD OUT"
                            retval["seatleft" + ii + "|" + jj] = ""
                            retval["seatname" + ii + "|" + jj] = ""
                        else:
                            retval["seatclass" + ii + "|" + jj] = " "
                            retval["seatbutton" + ii + "|" + jj] = "BOOK"
                            retval["seatleft" + ii + "|" + jj] = list[1] + " left"
                            retval["seatname" + ii + "|" + jj] = "buy_ticket"

                        retval["seatmoney" + ii + "|" + jj] = list[2]

        else: 
            retval = form.errors

        self.set_header("Content-Type","application/json")

        retval['errors'] = errors
        yield self.write(json.dumps(retval))


class TicketHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        BaseHandler.get(self)

    @tornado.gen.coroutine
    def post(self):
        command = self.request.arguments['command'][0].decode()
        print(command)
        link_command(command)
        link_read();


class BookingHandler(BaseHandler):
    @tornado.gen.coroutine
    @tornado.web.authenticated
    def get(self):
        self.render('booking.html', title="my booking", user=self.current_user,
                    info=UserSystem.queryById(self.current_user.decode()).fetchone())

    @tornado.gen.coroutine
    def post(self):
        errors = "Nothing"
        command = "query_order_all " + self.current_user.decode()
        print(command)
        link_command(command)
        n = int(link_read())
        retval = {'errors': 'Nothing'}
        if(n):
            errors = "Success"
            retval["cnt"] = n
            for i in range(0,n):
                ii = str(i)
                line = link_read()
                list = line.split()
                retval["seatcnt" + ii] = 1
                retval["code" + ii] = list[0]
                retval["startpos" + ii] = list[1]
                retval["seatdata" + ii + "|0"] = list[2]
                retval["start" + ii] = list[3]
                retval["endpos" + ii] = list[4]
                retval["end" + ii] = list[5]#多一天则加 <span class='day-plus'>+1d</span>
                if(list[6] == '1'): retval["end" + ii]+="<span class='day-plus'>+1d</span>"
                retval["time" + ii] = list[7]
                retval["seattype" + ii + "|0"] = list[8]

        self.set_header("Content-Type","application/json")

        retval['errors'] = errors
        yield self.write(json.dumps(retval))