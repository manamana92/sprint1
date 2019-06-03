CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing -lm
CC = gcc
all: sprint1 tests

sprint1: main.c encDec.c cipher.c keyExpansion.c
	$(CC) main.c encDec.c cipher.c keyExpansion.c -o sprint1 $(CFLAGS)

cipherDebug: cipher.c keyExpansion.c
	$(CC) cipher.c keyExpansion.c -o cipherDebug $(CFLAGS)

debug: main.c encDec.c cipher.c keyExpansion.c
	$(CC) main.c encDec.c cipher.c keyExpansion.c -g -ggdb3 -o sprint1Debug $(CFLAGS)

debugTests: tests.c encDec.c cipher.c keyExpansion.c
	$(CC) tests.c encDec.c cipher.c keyExpansion.c -g -ggdb3 -o testing $(CFLAGS)

tests: tests.c encDec.c cipher.c keyExpansion.c
	$(CC) tests.c encDec.c cipher.c keyExpansion.c -o testing $(CFLAGS)

clean:
	rm sprint1 sprint1Debug cipherDebug testing
