from PIL import Image
from Crypto.Cipher import AES
import os
import sys

def byte_xor(ba1, ba2):
    return bytes([_a ^ _b for _a, _b in zip(ba1, ba2)])

def pad(text):
    while len(text)%16 != 0:
        text += bytes([0])
    return text

def ECB(input_byte, key):
    cipher = AES.new(key, AES.MODE_ECB)
    cipher_image = b''
    for i in range(0, len(input_byte), 16):
        plaintext = input_byte[i:i+16]
        ciphertext = cipher.encrypt(plaintext)
        cipher_image += ciphertext

    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./ECB_encrypt.png')

def ECB_decrypt(input_byte, key):
    cipher = AES.new(key, AES.MODE_ECB)
    origin_image = b''
    for i in range(0, len(input_byte), 16):
        ciphertext = input_byte[i:i+16]
        plaintext = cipher.decrypt(ciphertext)
        origin_image += plaintext

    decrypt_image = Image.frombytes("RGB", ECB_encrypt.size, origin_image)
    decrypt_image.save('./ECB_decrypt.png')

def CBC(input_byte, key, IV):
    cipher = AES.new(key, AES.MODE_ECB)
    cipher_image = b''
    for i in range(0, len(input_byte), 16):
        plaintext = input_byte[i:i+16]
        xor = byte_xor(plaintext, IV)
        ciphertext = cipher.encrypt(xor)
        cipher_image += ciphertext
        IV = ciphertext
    
    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./CBC_encrypt.png')

def CBC_decrypt(input_byte, key, IV):
    cipher = AES.new(key, AES.MODE_ECB)
    origin_image = b''
    for i in range(0, len(input_byte), 16):
        ciphertext = input_byte[i:i+16]
        block = cipher.decrypt(ciphertext)
        plaintext = byte_xor(IV, block)
        origin_image += plaintext
        IV = ciphertext

    decrypt_image = Image.frombytes("RGB", CBC_encrypt.size, origin_image)
    decrypt_image.save('./CBC_decrypt.png')

def cool(input_byte, key, IV):
    cipher = AES.new(key, AES.MODE_ECB)
    cipher_image = b''
    n = 1
    for i in range(0, len(input_byte), 16):
        odd1 = IV
        odd2 = IV
        even1 = IV
        even2 = IV
        if n%2==1:  # 奇數個block
            plaintext = input_byte[i:i+16]
            xor_in = byte_xor(plaintext, odd1)
            block = cipher.encrypt(xor_in)
            xor_out = byte_xor(block, odd2)
            odd1 = xor_out
            odd2 = xor_in
            cipher_image += xor_out
        else:   # 偶數個block
            plaintext = input_byte[i:i+16]
            xor_in = byte_xor(plaintext, even1)
            block = cipher.encrypt(xor_in)
            xor_out = byte_xor(block, even2)
            even1 = block
            even2 = plaintext
            cipher_image += xor_out
        n+=1
        
    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./cool_encrypt.png')

def cool_decrypt(input_byte, key, IV):
    cipher = AES.new(key, AES.MODE_ECB)
    origin_image = b''
    n = 1
    for i in range(0, len(input_byte), 16):
        odd1 = IV
        odd2 = IV
        even1 = IV
        even2 = IV
        if n%2==0:  # 偶數個block
            ciphertext = input_byte[i:i+16]
            xor_in = byte_xor(ciphertext, odd1)
            block = cipher.decrypt(xor_in)
            plaintext = byte_xor(block, odd2)
            odd1 = plaintext
            odd2 = xor_in
            origin_image += plaintext
        else:   # 奇數個block
            ciphertext = input_byte[i:i+16]
            xor_in = byte_xor(ciphertext, even1)
            block = cipher.decrypt(xor_in)
            plaintext = byte_xor(block, even2)
            even1 = block
            even2 = ciphertext
            origin_image += plaintext
        n+=1

    decrypt_image = Image.frombytes("RGB", cool_encrypt.size, origin_image)
    decrypt_image.save('./cool_decrypt.png')

if __name__ == "__main__":
    mode = sys.argv[1]
    path = sys.argv[2]

    input_image = Image.open(path)
    input_byte = input_image.convert("RGB").tobytes()
    input_byte = pad(input_byte)    # 將不足16bytes的block補滿

    key = os.urandom(16)

    if mode == 'ECB':
        ECB(input_byte, key)
        ECB_encrypt = Image.open('./ECB_encrypt.png')
        ECB_encrypt_byte = ECB_encrypt.convert("RGB").tobytes()
        ECB_decrypt(ECB_encrypt_byte, key)
        print("Done")
    elif mode == 'CBC':
        IV = os.urandom(16)
        CBC(input_byte, key, IV)
        CBC_encrypt = Image.open('./CBC_encrypt.png')
        CBC_encrypt_byte = CBC_encrypt.convert("RGB").tobytes()
        CBC_decrypt(CBC_encrypt_byte, key, IV)
        print("Done")
    elif mode == 'cool':
        IV = os.urandom(16)
        cool(input_byte, key, IV)
        cool_encrypt = Image.open('./cool_encrypt.png')
        cool_encrypt_byte = cool_encrypt.convert("RGB").tobytes()
        cool_decrypt(cool_encrypt_byte, key, IV)
        print("Done")
    else:
        print('mode input error!')