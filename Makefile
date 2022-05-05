CC=gcc
CFLAGS=-Wall -Werror -lncurses -std=gnu99

main: main.c
	$(CC) -o main main.c $(CFLAGS)
