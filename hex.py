
def f(s):
    n = int(s, 2)
    print(hex(n));


if __name__ == "__main__":
    i = 0
    while True:
        s = input("")
        if s == "#":
            break
        if s == "##":
            print("");
            continue
        print('rk[%d] = ' % (i), end = "")
        f(s)
        i = i + 1

