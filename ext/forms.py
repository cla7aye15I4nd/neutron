from wtforms import validators
from wtforms import ValidationError
from wtforms import BooleanField, TextField, PasswordField
from wtforms_tornado import Form

from database import cursor

class UsernameValidators(object):
    def __init__(self):
        self.illegalMessage = 'Username contains illegal characters!'
        self.invalidMessage = 'No such Username!'

    def __call__(self, form, field):
        if not str(field.data).isalnum():
            raise ValidationError(self.illegalMessage)

class LoginForm(Form):
    username = TextField('Username', [
        validators.Required(message='Username is required.'),
        validators.Length(min=3, max=25, message='Username must be between 3 and 25 characters long'),
        UsernameValidators()
    ])
    password = PasswordField('New Password', [
        validators.Required(message='Password is required.'),
        validators.Length(min=8, max=64, message='Password must be between 8 and 64 characters long')
    ])
