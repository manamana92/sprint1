CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing -lm
CC = gcc
sprint1.exe : main.c
	$(CC) keyExpansion.c -o sprint1.exe $(CFLAGS)
