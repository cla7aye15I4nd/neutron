import requests as req

url = '127.0.0.1'
url = '108.61.126.68'
for x in range(1000):
    req.get('http://{}/login'.format(url))
    req.get('http://{}/register'.format(url))
