#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>

#include "network_helpers.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./a.out server_port");
    exit(1);
  }

  int socket_fd = Socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(atoi(argv[1]));

  fprintf(stderr, "Binding socket...\n");
  Bind(socket_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  fprintf(stderr, "Listening...\n");
  Listen(socket_fd, 50); // one connection at a time.

  fprintf(stderr, "Accepting...\n");
  struct sockaddr_in cli_addr;
  socklen_t cli_addr_len = sizeof(cli_addr);
  int conn_fd = Accept(socket_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
  fprintf(stderr, "Accepted!\n");
  sleep(1);

  fprintf(stderr, "Receiving...\n");
  for (int i = 0; i < 20; ++i) {
    char buf[512];
    bzero(buf, 512);

    int recv_bytes = Recv(conn_fd, buf, 512, 0);
    if (recv_bytes == 0) {
      fprintf(stderr, "Closing!");
      Close(conn_fd);
      break;
    }
    printf("Server received: %s\n", buf);

    bzero(buf, 512);
    snprintf(buf, 512, "Pong %d\n", i);
    Send(conn_fd, buf, strlen(buf), 0);
  }

  fprintf(stderr, "Exiting...");
  Close(socket_fd);
}
