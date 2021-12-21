#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network_helpers.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./tcp_client server_addr server_port");
    exit(1);
  }

  int socket_fd = Socket(PF_INET, SOCK_STREAM, 0);
  printf("socket created with fd %d\n", socket_fd);

  // initialize sockaddr_in struct
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(atoi(argv[2]));
  Inet_pton(socket_fd, argv[1], &dest_addr.sin_addr);

  fprintf(stderr, "Connecting...\n");
  Connect(socket_fd, (const struct sockaddr *)&dest_addr, sizeof(dest_addr));
  fprintf(stderr, "Successfully Connected.\n");
  sleep(5);

  for (int i = 0; i < 10; ++i) {
    char buf[512];
    bzero(buf, 512);
    snprintf(buf, 20, "Ping! %d\n", i);
    fprintf(stderr, "Sending...");
    Send(socket_fd, buf, strlen(buf), 0);

    bzero(buf, 512);
    Recv(socket_fd, buf, 512, 0);
    fprintf(stderr, "Client received: %s\n", buf);

    sleep(2);
  }

  Close(socket_fd);

  return 0;
}
