import json
import tornado.web
import tornado
import config
import os
import asyncio

from urllib.parse import urlencode
from verify import get, pack, unpack, gen
from base import BaseHandler
from forms import SearchForm2
from database import UserSystem
from link import link_command, link_read

def judge(blocks):

    return "Empty"


class ManageHandler(BaseHandler):
    @tornado.gen.coroutine
    def get(self):
        if(self.current_user == None or self.current_user.decode() != "2019"):
            BaseHandler.get(self)
        else:
            yield self.render('manage.html', title = 'manage', user = self.current_user)
        
        #yield self.render('manage.html', title = 'manage', user = self.current_user)

    @tornado.gen.coroutine
    def post(self):
        case = int(self.request.arguments['case'][0].decode())
        errors = "Failed"
        retval = {'errors': 'Failed'}
        if(case == 0):
            retval["case"] = 0
            form = SearchForm2(self.request.arguments)

            if form.validate():
                errors = "Nothing"
                search_id = self.request.arguments['search_id'][0].decode()
                command = "query_train " + search_id
                retval["train_id"] = search_id

                print(command)

                link_command(command)

                line = link_read()
                list = line.split()

                if(list[0] != '0'):
                    errors = "Success"
                    retval["train_name"] = list[1]
                    retval["train_type"] = list[2]
                    n = int(list[3])
                    retval["station_cnt"] = n
                    m = int(list[4])
                    retval["seat_cnt"] = m
                    cur = 5
                    for i in range(0,m):
                        ii = str(i)
                        retval["seattype" + ii] = list[cur]
                        cur+=1
                    retval["onsale"] = int(list[cur])

                    for i in range(0,n):
                        ii = str(i)
                        line = link_read()
                        list = line.split()
                        retval["station" + ii] = list[0]
                        for j in range(0,3):
                            jj = str(j)
                            retval["time" + ii + "s" + jj] = list[j + 1]
                        for j in range(0,m):
                            jj = str(j)
                            retval["price" + ii + "s" + jj] = list[j + 4]

            else: 
                retval = form.errors
        elif(case == 1):
            retval["case"] = 1
            form = SearchForm2(self.request.arguments)

            if form.validate():
                errors = "Hasone"
                search_id = self.request.arguments['search_id'][0].decode()
                command = "query_train " + search_id
                retval["train_id"] = search_id
                
                link_command(command)
                line = link_read()
                list = line.split()

                if(list[0] == '0'):
                    errors = "Success"
                else:
                    n = int(list[3])
                    for i in range(0,n):
                        link_read()
            else: 
                retval = form.errors
        elif(case == 2):
            command = "sale_train " + self.request.arguments['train_id'][0].decode()
            link_command(command)
            line = link_read()
            if(line == "0"):
                errors = "Failed"
            else:
                errors = "Success"
        elif(case == 3):
            retval["case"] = 3
            if(self.request.arguments['isempty'][0].decode()=="1"):
                errors = "Empty"
            else:
                train_id = self.request.arguments['train_id'][0].decode()
                train_name = self.request.arguments['train_id'][0].decode()
                command="modify_train "+self.request.arguments['train_id'][0].decode()+" "+self.request.arguments['train_name'][0].decode()+" "+self.request.arguments['train_type'][0].decode()+" "+self.request.arguments['stationcnt'][0].decode()+" "+self.request.arguments['seatcnt'][0].decode()+" "+self.request.arguments['seattype'][0].decode()+" "+self.request.arguments['stationlist'][0].decode()
                print(command)
                link_command(command)
                line = link_read()
                if(line == "0"):
                    errors = "Failed"
                else:
                    errors = "Success"
            
        elif(case == 4):
            command = "delete_train " + self.request.arguments['train_id'][0].decode()
            link_command(command)
            line = link_read()
            if(line == "0"):
                errors = "Failed"
            else:
                errors = "Success"
        elif(case == 5):
            retval["case"] = 5
            if(self.request.arguments['isempty'][0].decode()=="1"):
                errors = "Empty"
            else:
                train_id = self.request.arguments['train_id'][0].decode()
                train_name = self.request.arguments['train_id'][0].decode()
                command="modify_train "+self.request.arguments['train_id'][0].decode()+" "+self.request.arguments['train_name'][0].decode()+" "+self.request.arguments['train_type'][0].decode()+" "+self.request.arguments['stationcnt'][0].decode()+" "+self.request.arguments['seatcnt'][0].decode()+" "+self.request.arguments['seattype'][0].decode()+" "+self.request.arguments['stationlist'][0].decode()
                print(command)
                link_command(command)
                line = link_read()
                if(line == "0"):
                    errors = "Failed"
                else:
                    errors = "Success"

        self.set_header("Content-Type","application/json")

        retval['errors'] = errors
        yield self.write(json.dumps(retval))