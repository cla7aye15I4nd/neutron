import requests as req
import random
import re


way = re.compile("value=\"(.*)\"")

url = '127.0.0.1'
for x in range(100000):
    s = req.Session()
    r = s.get('http://{}:8000/register'.format(url))

    xsrf = re.findall(way, r.text)

    password = random.randint(10 ** 8, 10 ** 9)
    r = s.post('http://{}/register'.format(url), {'_xsrf' : xsrf,
                                                 'username' : random.randint(10 ** 8, 10 ** 9),
                                                 'password' : password,
                                                 'confirm' :  password,
                                                 'email'    : str(random.randint(10 ** 8, 10 ** 9)) + '@qq.com',
                                                 'phone'    : random.randint(10 ** 8, 10 ** 9)})
    print r.text
