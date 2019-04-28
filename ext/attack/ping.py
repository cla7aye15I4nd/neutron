import os
import base64
import random
import string
import asyncio

from Crypto.Cipher import AES
from Crypto import Random
from captcha.image import ImageCaptcha

from PIL import Image

code_len = 4
verify_secret = os.urandom(16)

async def get():
    return pack(str.encode(''.join([random.choice(string.ascii_lowercase + string.digits) for x in range(code_len)])))

async def pack(code):
    assert len(code) == code_len
    iv = Random.new().read(AES.block_size)
    engine = AES.new(verify_secret, AES.MODE_CFB, iv)
    return base64.b64encode(iv + engine.encrypt(os.urandom(16 - code_len) + code))


async def unpack(code):
    code = base64.b64decode(code)
    iv, cipher = code[:AES.block_size], code[AES.block_size:]
    engine = AES.new(verify_secret, AES.MODE_CFB, iv)
    return engine.decrypt(cipher)[-code_len:]

async def gen(code):
    img = ImageCaptcha()
    image = img.generate_image(code.decode())
    image.save(code, format='jpeg')
    with open(code, 'rb') as f:
        img = f.read()
    os.remove(code)
    return img

if __name__ == '__main__':
    async def main():
        for x in range(1000):
            await asyncio.create_task(pack(b'1234'))
    def run():
        for x in range(1000):
            pack(b'1234')
    run()
    asyncio.run(main())
