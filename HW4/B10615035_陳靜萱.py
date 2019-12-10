import random
import sys
def miller_rabin(num):
    if num == 2 or num == 3:
        return True
    if num < 2 or num%2==0:
        return False
    m=num-1
    k=0
    while m%2==0:
        m//=2
        k+=1
    for j in range(5):
        a=random.randrange(2,num-2)
        b=pow(a,m,num)
        if b!=1 and b!=(num-1):
            i=1
            while (i<k and b!=(num-1)):
                b=pow(b,2,num)
                if b==1:
                    return False
                i+=1
            if b!=(num-1):
                return False
    return True


# def miller_rabin(n):
#     if n == 2:
#         return True
#     if n % 2 == 0:
#         return False
#     k = 0
#     m = n - 1
#     while True:
#         quotient, remainder = divmod(m, 2)
#         if remainder == 1:
#             break
#         k += 1
#         m = quotient
#     def try_composite(a):
#         if pow(a, m, n) == 1:
#             return False
#         for i in range(k):
#             if pow(a, 2 ** i * m, n) == n - 1:
#                 return False
#         return True
#     for i in range(5):
#         a = random.randrange(2, n)
#         if try_composite(a):
#             return False
#     return True

def prime_generate():
    while True:
        num="1"
        for i in range(511):
            r=random.random()
            if r>0.5:
                r=1
            else:
                r=0
            num+=str(r)
        num+="1"
        num=int(num,2)
        if miller_rabin(num):
            return num
def square_and_multiply(num,e,n):
    b=bin(e)[2:]
    ans=num
    for i in range(1,len(b)):
        ans*=ans
        if b[i]=='1':
            ans*=num
        ans=ans%n
    return ans

def gcd(a,b): 
    if(b==0): 
        return a 
    else:
        return gcd(b,a%b) 

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def CRT(num,p,q,d,n):
    dp=d%(p-1)
    dq=d%(q-1)
    xp=square_and_multiply(num,dp,n)%p
    xq=square_and_multiply(num,dq,n)%q
    t=modinv(p,q)
    u=((xq-xp)*t)%q
    x=xp+p*u
    return(x)

if __name__ == "__main__":
    instruction=sys.argv[1]
    text=sys.argv[2]
    if instruction=="-e":
        p=prime_generate()
        q=prime_generate()
        n=p*q 
        phi_n=(p-1)*(q-1)
        e=0
        for i in range(2,phi_n):
            if (gcd(phi_n,i)==1):
                e=i
                break
        d=modinv(e,phi_n)
        ciphertext=""
        for i in range(len(text)):
            num=ord(text[i])
            y=square_and_multiply(num,e,n)%n
            ciphertext+=str(y)+","
        ciphertext=ciphertext[:-1]
        print("p=\n",p)
        print("\nq=\n",q)
        print("\nn=\n",n)
        print("\ne=\n",e)
        print("\nd=\n",d)
        print("\nciphertext=\n",ciphertext)
    elif instruction=="-d":
        p=sys.argv[3]
        q=sys.argv[4]
        d=sys.argv[5]
        n=int(p)*int(q)
        segment=""
        cipher_cut=[]
        for i in range(len(text)):
            if text[i]==",":
                cipher_cut.append(segment)
                segment=""
                continue
            segment+=text[i]
        else:
            cipher_cut.append(segment)
        plaintext=""
        for i in range(len(cipher_cut)):
            plaintext+=chr(CRT(int(cipher_cut[i]),int(p),int(q),int(d),n))
        print("plaintext=",plaintext)
    else:
        print("wrong input")