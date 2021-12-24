#include "network_helpers.h"
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage ./ip_client addr");
  }

  int socket_fd = Socket(AF_INET, SOCK_RAW, 0);
  for (int i = 0; i < 20; ++i) {
    char buf[512];
    bzero(buf, 512);

    Recvfrom(socket_fd, buf, 512);
    printf("Msg: %s\n", buf);
  }
}
