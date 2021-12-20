#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>

#include "network_helpers.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./a.out server_port");
    exit(1);
  }

  int socket_fd = Socket(PF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(atoi(argv[1]));

  fprintf(stderr, "Binding socket...\n");
  Bind(socket_fd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));

  fprintf(stderr, "Receiving...\n");
  for (int i = 0; i < 20; ++i) {
    char buf[512];
    bzero(buf, 512);

    Recvfrom(socket_fd, buf, 512);
    printf("Msg: %s\n", buf);
  }
}
