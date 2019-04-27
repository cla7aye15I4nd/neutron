import os
import base64

from Crypto.Cipher import AES
from Crypto import Random
from captcha.image import ImageCaptcha

from PIL import Image

code_len = 6
verify_secret = os.urandom(16)


def pack(code):
    assert len(code) == code_len
    iv = Random.new().read(AES.block_size)
    engine = AES.new(verify_secret, AES.MODE_CFB, iv)
    return base64.b64encode(iv + engine.encrypt(os.urandom(16 - code_len) + code))


def unpack(code):
    code = base64.b64decode(code)
    iv, cipher = code[:AES.block_size], code[AES.block_size:]
    engine = AES.new(verify_secret, AES.MODE_CFB, iv)
    return engine.decrypt(cipher)[-code_len:]

def gen(code):
    img = ImageCaptcha()
    image = img.generate_image(code.decode())
    image.save(code, format='jpeg')
    with open(code, 'rb') as f:
        img = f.read()
    os.remove(code)
    return img

if __name__ == '__main__':
    pass
