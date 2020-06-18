CC=gcc
CFLAGS=-I.  -std=gnu99

all: client server

MultModulo.o: MultModulo.h
	$(CC) -o MultModulo.o -c MultModulo.c $(CFLAGS)

server : server.c MultModulo.o MultModulo.h
	$(CC) -o server MultModulo.o server.c -lpthread $(CFLAGS)

client : client.c MultModulo.o MultModulo.h
	$(CC) -o client MultModulo.o client.c -lpthread $(CFLAGS)
    
clean :
	rm MultModulo.o client server