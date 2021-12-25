#include "network_helpers.h"
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage ./ip_client addr");
  }

  int socket_fd = Socket(AF_INET, SOCK_RAW, 0);
  /* int header_included = -1; */
  /* socklen_t option_size = sizeof(header_included); */
  /* getsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, &header_included, &option_size); */
  /* fprintf(stderr, "header include value: %d with size %d", header_included, option_size); */
  for (int i = 0; i < 20; ++i) {
    char buf[512];
    bzero(buf, 512);

    //Recvfrom returns the entire packet including the header lol.
    Recvfrom(socket_fd, buf, 512);
    fprintf(stderr, "Msg: ");
    for(int i = 0; i < 512; ++i) {
      fprintf(stderr, "%c", buf[i]);
    }
    fprintf(stderr, "\n");
  }
}
