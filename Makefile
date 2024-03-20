CC := g++
CFLAGS := -O3

SRCS := $(wildcard *.cpp)

main: $(SRCS)
	@$(CC) $(CFLAGS) $(SRCS) -o a.out

clean:
	@rm -rf a.out

sm3: main
	@./a.out sm3 `cat test/sm3.test`

sm4: main
	@./a.out sm4 `cat test/sm4.test`

.PHONY: clean sm3 sm4
