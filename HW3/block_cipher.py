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
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    input_byte = pad(input_byte)

    cipher_image = b''
    for i in range(0, len(input_byte), 16):
        one_block_text = input_byte[i:i+16]
        ciphertext = cipher.encrypt(one_block_text)
        cipher_image += ciphertext

    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./ECB_encrypt.png')

def ECB_decrypt(output_byte, key):
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    output_byte = pad(output_byte)

    origin_image = b''
    for i in range(0, len(output_byte), 16):
        one_block_text = output_byte[i:i+16]
        plaintext = cipher.decrypt(one_block_text)
        origin_image += plaintext

    decrypt_image = Image.frombytes("RGB", ECB_encrypt.size, origin_image)
    decrypt_image.save('./ECB_decrypt.png')

def CBC(input_byte, key, IV):
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    input_byte = pad(input_byte)

    cipher_image = b''
    for i in range(0, len(input_byte), 16):
        one_block_text = input_byte[i:i+16]
        xor = byte_xor(one_block_text, IV)
        ciphertext = cipher.encrypt(xor)
        IV = ciphertext
        cipher_image += ciphertext
    
    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./CBC_encrypt.png')

def CBC_decrypt(output_byte, key, IV):
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    output_byte = pad(output_byte)

    origin_image = b''
    for i in range(0, len(output_byte), 16):
        one_block_text = output_byte[i:i+16]
        to_xor = cipher.decrypt(one_block_text)
        plaintext = byte_xor(IV, to_xor)
        origin_image += plaintext
        IV = one_block_text

    decrypt_image = Image.frombytes("RGB", CBC_encrypt.size, origin_image)
    decrypt_image.save('./CBC_decrypt.png')

def cool(input_byte, key, IV):
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    input_byte = pad(input_byte)

    cipher_image = b''
    n = 1
    for i in range(0, len(input_byte), 16):
        odd1 = IV
        odd2 = IV
        even1 = IV
        even2 = IV
        if n%2==1:
            one_block_text = input_byte[i:i+16]
            xor_in = byte_xor(one_block_text, odd1)
            block = cipher.encrypt(xor_in)
            xor_out = byte_xor(block, odd2)
            odd1 = xor_out
            odd2 = xor_in
            cipher_image += xor_out
        else:
            one_block_text = input_byte[i:i+16]
            xor_in = byte_xor(one_block_text, even1)
            block = cipher.encrypt(xor_in)
            xor_out = byte_xor(block, even2)
            even1 = block
            even2 = one_block_text
            cipher_image += xor_out
        n+=1
        
    encrypt_image = Image.frombytes("RGB", input_image.size, cipher_image)
    encrypt_image.save('./cool_encrypt.png')

def cool_decrypt(output_byte, key, IV):
    cipher = AES.new(key.encode('utf8'), AES.MODE_ECB)

    output_byte = pad(output_byte)

    origin_image = b''
    n = 1
    for i in range(0, len(output_byte), 16):
        odd1 = IV
        odd2 = IV
        even1 = IV
        even2 = IV
        if n%2==0:
            one_block_text = output_byte[i:i+16]
            xor_in = byte_xor(one_block_text, odd1)
            block = cipher.decrypt(xor_in)
            plaintext = byte_xor(block, odd2)
            odd1 = plaintext
            odd2 = xor_in
            origin_image += plaintext
        else:
            one_block_text = output_byte[i:i+16]
            xor_in = byte_xor(one_block_text, even1)
            block = cipher.decrypt(xor_in)
            plaintext = byte_xor(block, even2)
            even1 = block
            even2 = one_block_text
            origin_image += plaintext
        n+=1

    decrypt_image = Image.frombytes("RGB", cool_encrypt.size, origin_image)
    decrypt_image.save('./cool_decrypt.png')

if __name__ == "__main__":
    mode = sys.argv[1]
    path = sys.argv[2]

    input_image = Image.open(path)
    input_byte = input_image.convert("RGB").tobytes()

    key = '0123456789ABCDEF'

    if mode == 'ECB':
        ECB(input_byte, key)
        ECB_encrypt = Image.open('./ECB_encrypt.png')
        ECB_output_byte = ECB_encrypt.convert("RGB").tobytes()
        ECB_decrypt(ECB_output_byte, key)
        print("Done")
    elif mode == 'CBC':
        IV = os.urandom(16)
        CBC(input_byte, key, IV)
        CBC_encrypt = Image.open('./CBC_encrypt.png')
        CBC_output_byte = CBC_encrypt.convert("RGB").tobytes()
        CBC_decrypt(CBC_output_byte, key, IV)
        print("Done")
    elif mode == 'cool':
        IV = os.urandom(16)
        cool(input_byte, key, IV)
        cool_encrypt = Image.open('./cool_encrypt.png')
        cool_output_byte = cool_encrypt.convert("RGB").tobytes()
        cool_decrypt(cool_output_byte, key, IV)
        print("Done")
    else:
        print('mode input error!')