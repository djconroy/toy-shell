CC=gcc
CFLAGS=-g -Wall

all: shell

shell: main.o prompt.o tokenize.o
	$(CC) main.o prompt.o tokenize.o -o shell

main.o: main.c prompt.h tokenize.h
	$(CC) -c $(CFLAGS) main.c

prompt.o: prompt.c prompt.h
	$(CC) -c $(CFLAGS) prompt.c

tokenize.o: tokenize.c tokenize.h
	$(CC) -c $(CFLAGS) tokenize.c

clean:
	rm -rf *.o shell
