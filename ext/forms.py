from wtforms import ValidationError, Field, validators
from wtforms import BooleanField, TextField, PasswordField
from wtforms_tornado import Form

from database import UserSystem

class UsernameLoginValidators(object):
    def __init__(self):
        self.illegalMessage = 'Username contains illegal characters!'
        self.invalidMessage = 'Wrong Username Or Password!'

    def __call__(self, form, field):
        if not str(field.data).isalnum():
            raise ValidationError(self.illegalMessage)
        if not UserSystem.checkLogin(field.data, form.password.data):
            raise ValidationError(self.invalidMessage)

class LoginForm(Form):
    password = PasswordField('New Password', [
        validators.Required(message='Password is required.'),
        validators.Length(min=8, max=64, message='Password must be between 8 and 64 characters long')
    ])
    username = TextField('Username', [
        validators.Required(message='Username is required.'),
        validators.Length(min=3, max=15, message='Username must be between 3 and 15 characters long'),
        UsernameLoginValidators()
    ])

class UsernameRegisterValidators(object):
    def __init__(self):
        self.illegalMessage = 'Username contains illegal characters!'
        self.invalidMessage = 'Username has been used!'

    def __call__(self, form, field):
        if not str(field.data).isalnum():
            raise ValidationError(self.illegalMessage)
        if UserSystem.isExist({'username' : field.data}):
            raise ValidationError(self.invalidMessage)

class EmailRegisterValidator():
    def __init__(self, message=None):
        if message == None:
            self.message = "The email has been registered"
    
    def __call__(self, form, field):
        if UserSystem.isExist({'email': field.data}):
            raise ValidationError(self.message)

class PhoneRegisterValidator():
    def __init__(self):
        self.illegalMessage = "Please input right phone number"
        self.invalidMessage = "The phone number has been registered"
    
    def __call__(self, form, field):
        if not field.data.isnumeric():
            raise ValidationError(self.illegalMessage)
        if UserSystem.isExist({'phone': field.data}):
            raise ValidationError(self.invalidMessage)

class ConfirmValidator():
    def __init__(self):
        self.message = "The two passwords you typed do not match"
    
    def __call__(self, form, field):
        if field.data != form.password.data:
            raise ValidationError(self.message)

class RegisterForm(Form):
    username = TextField('Username', [
        validators.Required(message='Username is required.'),
        validators.Length(min=3, max=15, message='Username must be between 3 and 15 characters long'),
        UsernameRegisterValidators()
    ])
    password = PasswordField('New Password', [
        validators.Required(message='Password is required.'),
        validators.Length(min=8, max=64, message='Password must be between 8 and 64 characters long')
    ])
    confirm = PasswordField('Confirm Password', [
        validators.Required(message='Confirm password is required.'),
        ConfirmValidator()
    ])
    email = TextField('Email', [
        validators.Required(message='Email is required.'),
        validators.Email(),
        EmailRegisterValidator()
    ])
    phone = TextField('Phone Number', [
        validators.Required(message='Phone number is required.'),
        PhoneRegisterValidator()
    ])

class UsernameUpdateValidators(Form):
    def __init__(self):
        self.illegalMessage = 'Username contains illegal characters!'
        self.invalidMessage = 'Username has been used!'

    def __call__(self, form, field):
        if not str(field.data).isalnum():
            raise ValidationError(self.illegalMessage)

        if UserSystem.count({'username' : field.data}) != 0 and form.originUsername.data != field.data:
            raise ValidationError(self.invalidMessage)

class EmailUpdateValidator():
    def __init__(self, message=None):
        if message == None:
            self.message = "The email has been registered"
    
    def __call__(self, form, field):
        if UserSystem.count({'email': field.data}) != 0 and form.originEmail.data != field.data:
            raise ValidationError(self.message)

class PhoneUpdateValidator():
    def __init__(self):
        self.illegalMessage = "Please input right phone number"
        self.invalidMessage = "The phone number has been registered"
    
    def __call__(self, form, field):
        if not field.data.isnumeric():
            raise ValidationError(self.illegalMessage)
        if UserSystem.count({'phone': field.data}) != 0 and form.originPhone.data != field.data:
            raise ValidationError(self.invalidMessage)

class UpdateForm(Form):
    originUsername = TextField('Origin username', [])
    originEmail    = TextField('Origin email', []) 
    originPhone    = TextField('Origin phone', []) 
    username = TextField('Username', [
        validators.Required(message='Username is required.'),
        validators.Length(min=3, max=15, message='Username must be between 3 and 15 characters long'),
        UsernameUpdateValidators()
    ])
    email = TextField('Email', [
        validators.Required(message='Email is required.'),
        validators.Email(),
        EmailUpdateValidator()
    ])
    phone = TextField('Phone Number', [
        validators.Required(message='Phone number is required.'),
        PhoneUpdateValidator()
    ])
