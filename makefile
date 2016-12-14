CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing -lm
CC = gcc
all: sprint1

sprint1: encDec.c cipher.c keyExpansion.c
	$(CC) encDec.c cipher.c keyExpansion.c -o sprint1 $(CFLAGS)

debug: encDec.c cipher.c keyExpansion.c
	$(CC) encDec.c cipher.c keyExpansion.c -g -o sprint1Debug $(CFLAGS)
