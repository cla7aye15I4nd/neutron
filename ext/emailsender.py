import json
import smtplib
from email.mime.text import MIMEText
from email.header import Header

import time

def Sendmail(receiver, code):
    
    sender={"pass": "1Y7LZPkzjpRatxwK","mail": "ulita0@163.com","user": "ulita0@163.com", "host": "smtp.163.com"}

    msg = """
        <h1>Neutron E-mail verification</h1>
        <p>Welcome!</p>
        <br />
        <p>Here's your verification code for your new Neutron account:</p>
        <p>{}</p>
        <p>Please return to Neutron and enter this code to complete your registration and unlock your full acount.</p>
        <p>We'll also import any bookings you've made with this email address.</p>
    """.format(code)
    
    message = MIMEText(msg, 'html','utf-8')
    message['Subject'] = 'E-mail verification ' + str(code)
    message['From']    = sender['mail']
    message['To']      = receiver
    message['Cc']      = sender['mail']

    try:
        
        smtpObj = smtplib.SMTP_SSL(sender['host'], 465)
        smtpObj.set_debuglevel(1)
        smtpObj.helo(sender['host'])
        smtpObj.ehlo(sender['host'])

        smtpObj.login(sender['user'], sender['pass']) 
        smtpObj.sendmail(sender['mail'], receiver, message.as_string()) 
        smtpObj.quit()

        
        
        print('successful ', receiver)
    except smtplib.SMTPException as e:
        print(e)


def Sendmail2(receiver, code):
    
    sender={"pass": "1Y7LZPkzjpRatxwK","mail": "ulita0@163.com","user": "ulita0@163.com", "host": "smtp.163.com"}

    msg = """
        <h1>Neutron E-mail verification</h1>
        <p>Welcome!</p>
        <br />
        <p>Here's your verification code for your new Neutron account:</p>
        <p>{}</p>
        <p>Please return to Neutron and enter this code to complete your registration and unlock your full acount.</p>
        <p>We'll also import any bookings you've made with this email address.</p>
    """.format(code)
    
    message = MIMEText(msg, 'html','utf-8')
    message['Subject'] = 'E-mail verification ' + str(code)
    message['From']    = sender['mail']
    message['To']      = receiver
    message['Cc']      = sender['mail']

    try:
        
        smtpObj = smtplib.SMTP_SSL(sender['host'], 465)
        smtpObj.set_debuglevel(1)
        smtpObj.helo(sender['host'])
        smtpObj.ehlo(sender['host'])

        smtpObj.login(sender['user'], sender['pass']) 
        smtpObj.sendmail(sender['mail'], receiver, message.as_string()) 
        smtpObj.quit()

        
        
        print('successful ', receiver)
    except smtplib.SMTPException as e:
        print(e)