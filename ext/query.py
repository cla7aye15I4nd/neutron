import tornado.web

from base import BaseHandler

class TrainHandler(BaseHandler):
    def get(self):
        self.render('trains.html', title='Trains')