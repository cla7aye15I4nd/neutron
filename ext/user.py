import tornado.web
from base import BaseHandler

from database import UserSystem
from forms import UpdateForm
import json


class ProfileHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.render('profile.html', title="profile", user=self.current_user,
                    info=UserSystem.queryById(self.current_user.decode()).fetchone())

    @tornado.web.authenticated
    def post(self):
        arg = self.request.arguments
        result = UserSystem.queryById(self.current_user.decode()).fetchone()

        diff = arg['username'][0].decode() != result[1]
        diff += arg['email'][0].decode() != result[3]
        diff += arg['phone'][0].decode() != result[4]

        if diff == 0:
            self.write(json.dumps({'errors': 'Nothing'}))
        else:
            arg['originUsername'] = [result[1]]
            arg['originEmail'] = [result[3]]
            arg['originPhone'] = [result[4]]

            form = UpdateForm(arg)

            errors = "Failed"
            if form.validate():
                UserSystem.update(result[0], arg['username'][0].decode(), arg['email'][0].decode(), arg['phone'][0].decode())
                errors = "Success"
            self.set_header("Content-Type", "application/json")

            retval = form.errors
            retval['errors'] = errors
            self.write(json.dumps(retval))


class SettingHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.render('setting.html', title="setting", user=self.current_user,
                    info =  UserSystem.queryById(self.current_user.decode()).fetchone())


class BookingHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.render('booking.html', title="my booking", user=self.current_user,
                    info =  UserSystem.queryById(self.current_user.decode()).fetchone())
