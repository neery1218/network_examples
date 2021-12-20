CC=gcc
FLAGS=-Wall -g

all: udp_client udp_server

udp_client: udp_client.c
	$(CC) $(FLAGS) -o udp_client udp_client.c

udp_server: udp_server.c
	$(CC) $(FLAGS) -o udp_server udp_server.c

.PHONY: clean

clean:
	rm -f udp_client udp_server
