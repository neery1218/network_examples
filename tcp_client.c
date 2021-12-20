#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "network_helpers.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./tcp_client server_addr server_port");
    exit(1);
  }


  int socket_fd = Socket(PF_INET, SOCK_DGRAM, 0);
  printf("socket created with fd %d\n", socket_fd);

  // initialize sockaddr_in struct
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(atoi(argv[2]));
  int rv;
  if ((rv = inet_pton(AF_INET, argv[1], &dest_addr.sin_addr)) != 1) {
    printf("pton exited with code %d\n", rv);
    exit(1);
  }

  for (int i = 0; i < 100; ++i) {
    char buf[512];
    snprintf(buf, 20, "Hello World! %d\n", i);
    buf[20] = 0;

    int sent_bytes =
        sendto(socket_fd, buf, strlen(buf), 0, (const struct sockaddr *)&dest_addr,
               sizeof(dest_addr));

    if (sent_bytes < 0) {
      printf("send failed: errno: %d\n", errno);
    }
    printf("Sent %d bytes!\n", sent_bytes);
    sleep(5);
  }

  return 0;
}
