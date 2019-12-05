import math
import random
import sys

def square_and_multiply(x, e, n): # z = x^e mod n
    e = '{0:b}'.format(e) # 字串格式化 (decimal->binary) e=3 -> e='11'
    output = 1
    for i in range(len(e)):
        output = (output**2) % n   # 進位: (output^2)
        if(e[i]=='1'):
            output = (output*x) % n   # 加1: *(x)
    return output

def find_large_prime(bit):
    temp = []
    temp.append(1)
    for i in range(bit-2):
        rand = random.randint(0, 1)
        temp.append(rand)
    temp.append(1)
    
    arr = [str(j) for j in temp]
    prime = ''.join(arr)
    b = int(prime, 2)
    return b

def miller_rabin_test(n):
    if n == 2:
        return True
    if n%2 == 0:
        return False  
    # n-1 = 2^s * d   ,  找 s  d
    s, d = 0, n-1 # d is odd  
    while d%2 == 0:
        s += 1
        d //= 2
    # 循環測試
    # a^(n-1)≡1(mod n) 成立 -> 可能是prime ; 不成立 -> 一定是composite (witness)
    count = 10
    for i in range(count):  # 檢查次數
        a = random.randrange(1, n-1)  # [1, n-1] 之間選出 a
        x = square_and_multiply(a, d, n)  # x = a^d (mod n)
        if x == 1 or x == n-1:  # 可能是prime 繼續確認
            continue
        for _ in range(s-1):  # [0, s-1] 的範圍 x^2
            x = square_and_multiply(x, 2, n)  # x = x^(2*i) (mod n)
            if x == n-1:  # 可能是prime 繼續確認
                break
        else:  # for [0, s-1] 跑完仍無法判定是prime, 就是composite
            return False
    return True

def prime(bit):
    while 1:
        prime = find_large_prime(bit)
        if miller_rabin_test(prime) == True:
            return prime

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)
    
def inverse(e, phi_n):
    g, x, y = egcd(e, phi_n)
    if g != 1:
        raise Exception('no inverse')
    else:
         return x % phi_n

def decrypt_with_CRT(y, d, p, q):
    # step 1: 轉成 CRT domain (n -> p, q)
    # step 2: reduce exp (d -> dp, dq)
    dp = d % (p-1)
    dq = d % (q-1)
    q_inv = inverse(q, p)
    x1 = pow(y, dp, p)
    x2 = pow(y, dq, q)
    # step 3: inverse, CRT domain -> normal domain
    h = (q_inv*(x1-x2)) % p
    x = x2 + h*q
    return x

def Encrypt():
    # 1. choose large prime p, q
    # 2. compute n
    plaintext = sys.argv[2]
    p = prime(512)
    q = prime(513)  
    n = p * q
    if len(str(n)) < len(str(plaintext)):
        raise Exception('n不能小於plaintext')      
    # 3. compute phi_n
    phi_n = (p-1)*(q-1) 
    # 4. select e
    e = 0
    for i in range(2, phi_n):
        if math.gcd(i, phi_n) == 1:
            e = i
            break
    # 5. compute private key d
    d = inverse(e, phi_n)
    # Encrypt
    ciphertext = []
    for i in plaintext:
        text = square_and_multiply(ord(i), e, n)
        ciphertext.append(text)

    # 將ciphertext轉成string
    arr = [str(j) for j in ciphertext]
    ciphertext = ','.join(arr)
    print("ciphertext = ", ciphertext)
    print("p =", p)
    print("q =", q)
    print("n =", n)
    print("n bit =", len(bin(n))-2)
    print("e =", e)
    print("d =", d)

def Decrypt():
    cipher_text = sys.argv[2]
    arr = cipher_text.split(',')
    ciphertext = []
    for i in arr:
        ciphertext.append(int(i))
    print("input d = ")
    d = int(input())
    print("input p = ")
    p = int(input())
    print("input q = ")
    q = int(input())

    # Decrypt
    plaintext = ''
    for i in ciphertext:
        temp = decrypt_with_CRT(i, d, p, q)
        plaintext += (chr(temp))
    print("plaintext = ", plaintext)

if __name__ == "__main__":
    cipher = sys.argv[1]
    if cipher == '-e':
        Encrypt()
    elif cipher == '-d':
        Decrypt()
    else:
        print('please choose -e/-d')