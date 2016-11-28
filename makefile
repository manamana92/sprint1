CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing -lm
CC = gcc
sprint1.exe : cipher.c keyExpansion.c
	$(CC) cipher.c keyExpansion.c -o sprint1.exe $(CFLAGS)
