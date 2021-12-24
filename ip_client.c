#include "network_helpers.h"
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage ./ip_client addr");
  }

  int socket_fd = Socket(AF_INET, SOCK_RAW, 0);
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  /* dest_addr.sin_port = htons(atoi(argv[2])); */
  Inet_pton(socket_fd, argv[1], &dest_addr.sin_addr);

  for (int i = 0; i < 100; ++i) {
    char buf[100];
    snprintf(buf, 100, "ping %d", i);
    fprintf(stderr, "Sending...");
    SendTo(socket_fd, buf, strlen(buf), (const struct sockaddr*)&dest_addr, sizeof(dest_addr));
    sleep(3);
  }

}
