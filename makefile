CC = gcc
CFLAGS = -Wall -g -I.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

all: server client
	
server: server.o
	$(CC) -o server server.o $(CFLAGS)
	
client: client.o
	$(CC) -o client client.o $(CFLAGS)
	

.PHONY: clean
clean:
	rm -f *.o server
	rm -f *.o client

