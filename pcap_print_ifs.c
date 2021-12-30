#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
  switch (sa->sa_family) {
  case AF_INET:
    inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
    break;

  case AF_INET6:
    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
    break;

  default:
    snprintf(s, maxlen, "Unknown AF: %u", sa->sa_family);
    return NULL;
  }

  return s;
}

int main(int argc, char **argv) {
  char errbuf[PCAP_ERRBUF_SIZE];

  // if stands for interfaces
  pcap_if_t *alldevsp;
  if (pcap_findalldevs(&alldevsp, errbuf) < 0) {
    perror("findalldevs failed.");
  }

  pcap_if_t *head = alldevsp;
  while (head) {
    if ((head->flags & (PCAP_IF_UP | PCAP_IF_RUNNING)) &&
        (head->flags & PCAP_IF_CONNECTION_STATUS) == PCAP_IF_CONNECTION_STATUS_CONNECTED) {
      printf("name: %s , desc: %s, flags: %#08X\n", head->name, head->description, head->flags);
      struct pcap_addr *addr_head = head->addresses;
      while (addr_head) {
        char human_addr[100];
        get_ip_str(addr_head->addr, human_addr, 100);
        printf("\taddr: %s\n", human_addr);
        addr_head = addr_head->next;
      }
    }
    head = head->next;
  }
  pcap_freealldevs(alldevsp);
}
