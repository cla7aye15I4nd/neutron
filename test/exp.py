import sqlite3
from random import randint

conn = sqlite3.connect('neutron.db')
c = conn.cursor()

# for x in range(500000):
#     c.execute("INSERT INTO user (username, password, email, phone) VALUES('{}','{}','{}','{}')".format(random.randint(10 ** 8, 10 ** 9),random.randint(10 ** 8, 10 ** 9),random.randint(10 ** 8, 10 ** 9),random.randint(10 ** 8, 10 ** 9)))
# conn.commit()

for x in range(100000):
    y = randint(1, 100000)
    command = "UPDATE user SET username = '{}', email = '{}', phone = '{}' WHERE id = {}".format(randint(1, 100), randint(1, 100), randint(1, 100), 1231)
    c.execute(command)
# conn.commit()
