CC=gcc
FLAGS=-Wall -g

all: udp_client udp_server tcp_client tcp_server ip_client ip_server

ip_client: ip_client.c
	$(CC) $(FLAGS) -o ip_client ip_client.c

ip_server: ip_server.c
	$(CC) $(FLAGS) -o ip_server ip_server.c

udp_client: udp_client.c
	$(CC) $(FLAGS) -o udp_client udp_client.c

udp_server: udp_server.c
	$(CC) $(FLAGS) -o udp_server udp_server.c

tcp_client: tcp_client.c
	$(CC) $(FLAGS) -o tcp_client tcp_client.c

tcp_server: tcp_server.c
	$(CC) $(FLAGS) -o tcp_server tcp_server.c

.PHONY: clean

clean:
	rm -f udp_client udp_server tcp_client tcp_server ip_client ip_server
