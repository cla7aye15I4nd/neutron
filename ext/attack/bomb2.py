import requests as req
import random
import re


way = re.compile("csrf_token = \"(.*)\"")

url = '108.61.126.68'
for x in range(1):
    s = req.Session()
    r = s.get('http://{}/register'.format(url))

    xsrf = re.findall(way, r.text)

    #print r.text, xsrf
    password = random.randint(10 ** 8, 10 ** 9)
    r = s.post('http://{}/api/register'.format(url), {'X-CSRFToken' : xsrf,
                                                      'name' : random.randint(10 ** 8, 10 ** 9),
                                                      'password' : password,
                                                      'email'    : str(random.randint(10 ** 8, 10 ** 9)) + '@qq.com',
                                                      'phone'    : random.randint(10 ** 8, 10 ** 9)})
    print r.text
