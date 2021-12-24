#include "network_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./a.out server_addr server_port");
    exit(1);
  }

  int socket_fd = Socket(PF_INET, SOCK_DGRAM, 0);
  printf("socket created with fd %d\n", socket_fd);

  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  Inet_pton(socket_fd, argv[1], &dest_addr.sin_addr);

  for (int i = 0; i < 100; ++i) {
    char buf[512];
    snprintf(buf, 20, "Hello World! %d\n", i);
    buf[20] = 0;

    int sent_bytes =
        SendTo(socket_fd, buf, strlen(buf), (const struct sockaddr *)&dest_addr,
               sizeof(dest_addr));
    printf("Sent %d bytes!\n", sent_bytes);
    sleep(5);
  }

  return 0;
}
