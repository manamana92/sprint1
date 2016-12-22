CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing -lm
CC = gcc
all: sprint1

sprint1: main.c encDec.c cipher.c keyExpansion.c
	$(CC) main.c encDec.c cipher.c keyExpansion.c -o sprint1 $(CFLAGS)

cipherDebug: cipher.c keyExpansion.c
	$(CC) cipher.c keyExpansion.c -o cipherDebug $(CFLAGS)

debug: main.c encDec.c cipher.c keyExpansion.c
	$(CC) main.c encDec.c cipher.c keyExpansion.c -g -o sprint1Debug $(CFLAGS)

clean:
	rm a.out sprint1 sprint1Debug cipherDebug
