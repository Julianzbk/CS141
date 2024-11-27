import math
UPPER_LIMIT = 100000

def u(n):
    return 0 if n < 0 else 1

def y_1(n):
    if (n < 0):
        return 0
    return u(n) + 0.75*y_1(n-1) - 0.125*y_1(n-2)

def x_2(n):
    return pow(3/2, n) * u(n)

def y_2(n):
    if n <= -1:
        return 0
    return x_2(n) + 0.5*y_2(n-1)

def close_enough(a, b, diff = 1e-10):
    return a - diff <= b or a + diff >= b

def test():
    y = lambda n: y_1(n)
    check = lambda n: 8/3 + 2 - pow(1/4, n) * u(n)
    for n in range(0, 100):
        try:
            print(f"y[{n}] = {y(n)}")
            print(f"c[{n}] = {check(n)}")
            if close_enough(y(n), check(n)):
                print("Good check\n")
        except ZeroDivisionError:
            print("*DivideByZero*\n")

if __name__ == "__main__":
    test()
