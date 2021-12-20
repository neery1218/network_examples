#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int Socket(int domain, int type, int protocol) {
  int socket_fd;
  if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr, "socket_fd %d failed with errno %d", socket_fd, errno);
    exit(1);
  }
  return socket_fd;
}

void Connect(int socket, const struct sockaddr *address,
             socklen_t address_len) {
  if (connect(socket, address, address_len) < 0) {
    fprintf(stderr, "connect failed on socket %d with errno %d\n", socket,
            errno);
    exit(1);
  }
}

void Inet_pton(int af, const char *src_addr, void *dst_addr) {
  int rv;
  if ((rv = inet_pton(AF_INET, src_addr, dst_addr)) != 1) {
    fprintf(stderr, "pton exited with code %d\n", rv);
    exit(1);
  }
}

int SendTo(int socket, const char *buffer, size_t length, 
    const struct sockaddr *dest_addr, socklen_t dest_len) {
  int sent_bytes = sendto(socket, buffer, length, 0, dest_addr, dest_len);
  if (sent_bytes < 0) {
    fprintf(stderr, "send failed: errno: %d\n", errno);
  }

  return sent_bytes;
}

void Bind(int socket, const struct sockaddr *address, socklen_t address_len) {
  if (bind(socket, address, address_len) < 0) {
    fprintf(stderr, "Bind failed with errno: %d\n", errno);
    exit(1);
  }
}

ssize_t Recvfrom(int socket, void *buffer, size_t length) {
  int recv_bytes;
  if ((recv_bytes = recvfrom(socket, buffer, length, 0, NULL, 0)) < 0) {
    fprintf(stderr, "Recv call failed with errno: %d", errno);
    exit(1);
  }
  return recv_bytes;
}
