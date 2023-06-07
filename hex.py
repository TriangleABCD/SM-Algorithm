
def f(s):
    n = int(s, 2)
    print('{:08x}'.format(n), end=" ")
    

if __name__ == "__main__":
    i = 0
    k = 0
    while True:
        k = k + 1
        s = input("")
        if s == "#":
            break
        if s == "##":
            print("");
            i = 0
            continue
        if (k-1) % 8 == 0:
            print('{:02d}'.format((k-1)//8), end=" ")
        f(s)
        i = i + 1
        if i % 8 == 0:
            print("")

