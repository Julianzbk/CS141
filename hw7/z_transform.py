from EECS50 import u
from math import cos, pi
import cmath

LIMIT = 1000
CHECK_RANGE = range(-20, 20)
j = complex(0, 1)

Q4 = [lambda n: pow(0.7, n) * u(n + 7),
      lambda n: pow(-0.7, n) * u(-n - 7),
      lambda n: pow(0.7, -n) * u(n + 7),
      lambda n: cmath.exp(j * pi/3 * n) * u(-n + 2),
      lambda n: cos(pi/3 * n) * u(-n + 2)]

def unilateral(x):
    def transformed_x(z):
        sum = 0
        for n in range(0, LIMIT):
            sum += x(n) * pow(z, -n)
        return sum
    return transformed_x

def bilateral(x):
    def transformed_x(z):
        sum = 0
        for n in range(-LIMIT, LIMIT):
            sum += x(n) * pow(z, -n)
        return sum
    return transformed_x

def ROC(xb):
    for z in range(-40, 40):
        z /= 4
        try:
            out = xb(z)
            if (is_infinity(out)):
                print(f"{z} is Not convergent: {out}")
            else:
                print(f"xb[{z}] = {out}")
        except ZeroDivisionError:
            print(f"{z} is Not convergent: Divide by 0")
        except OverflowError:
            print(f"{z} is Not convergent: Numeric overflow")

def close_enough(a, b, diff = 1e-10):
    return a - diff <= b or a + diff >= b

def is_infinity(n):
    return abs(n) > 1e100

def test():
    xb = bilateral(y_1)
    check = lambda z: 8/3 + 2 - pow(1/4, z) * u(z)
    for z in CHECK_RANGE:
        try:
            print(f"xb[{z}] = {xb(z)}")
            print(f"ch[{z}] = {check(z)}")
            if close_enough(xb(z), check(z)):
                print("Good check\n")
        except ZeroDivisionError:
            print("*DivideByZero*\n")

    
if __name__ == "__main__":
    for i in range(len(Q4)):
        print(chr(i + 97), ":", sep='')
        ROC(bilateral(Q4[i]))
        print()