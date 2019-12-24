import math
import random
import sys
from hashlib import sha1

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
        x = pow(a, d, n)  # x = a^d (mod n)
        if x == 1 or x == n-1:  # 可能是prime 繼續確認
            continue
        for _ in range(s-1):  # [0, s-1] 的範圍 x^2
            x = pow(x, 2, n)  # x = x^(2*i) (mod n)
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
        
def bitGenerator(bit):
    temp = []
    for i in range(bit):
        rand = random.randint(0, 1)
        temp.append(rand)
    arr = [str(j) for j in temp]
    prime = ''.join(arr)
    b = int(prime, 2)
    return b
    
if __name__ == "__main__":
    plaintext = sys.argv[1]
    ### generate key
    print("-----Generate Key-----")
    # p(1024) = q(160) * k + 1
    flag = False
    while 1:
        q = prime(160)
        count = 0
        while count < 100:
            k = bitGenerator(1024-160)
            p = k * q + 1
            if (p.bit_length() == 1024 and miller_rabin_test(p)):
                flag = True
                break
            count += 1
        if flag:
            break
    print("p: ", p)
    print("p_bits: ",p.bit_length())
    print("q: ", q)
    print("q_bits: ",q.bit_length())

    # a = h^((p-1)/q)
    h = random.randint(2, p-1) 
    a = pow(h, (p-1)//q, p)
    print("a: ", a)
    
    # 1 < d < q
    d = random.randint(1, q)
    print("d: ", d)
    
    # B = a^d mod p
    B = pow(a, d, p)
    print("B: ", B)
    
    ### signature
    print("-----Signature-----")
    SHA = sha1(plaintext.encode('utf-8')).hexdigest()
    print("SHA: ", SHA)
    # 1 < Ke < q
    Ke = random.randint(1, q)
    print("Ke: ", Ke)
    Ke_inv = inverse(Ke, q)
    print("Ke_inv: ", Ke_inv)
    # r = a^Ke mod p mod q
    r = pow(a, Ke, p) % q
    print("r: ", r)
    # s = (SHA + d*r)*Ke_inv mod q
    s = []
    for i in SHA:
        i = ord(i)
        s_n = ((i + d*r) * Ke_inv) % q
        s.append(s_n)
    print("s: ", s)

    ### verify
    print("-----Verify-----")
    v = []
    for i in range(len(s)):
        w = inverse(s[i], q)
        u1 = (w * ord(SHA[i])) % q
        u2 = (w * r) % q
        v_n = ((pow(a, u1, p) * pow(B, u2, p)) % p) % q
        v.append(v_n)
    print("v: ", v)

    ### Compare
    print("-----Output (r==v -> valid)-----")
    for i in range(len(v)):
        if r != v[i]:
            print('invalid')
            break
    else:
        print('valid')