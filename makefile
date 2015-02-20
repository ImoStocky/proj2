CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

proj2:proj2.o
	$(CC) $(CFLAGS) proj2.o -lm -o proj2
