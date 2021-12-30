/* sniff IP packets on en0 interface */
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ipv4_header {
  uint8_t version_and_ihl;
  uint8_t tos;
  uint16_t total_length;
  uint16_t id;
  uint16_t flags_and_frag_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t source_ip;
  uint32_t dest_ip;
};

struct ipv4_packet {
  const struct ipv4_header *header;
  const void *data;
  size_t data_len;
};

/*
 * Assumptions:
 *      data: points to start of IP packet.
 *      packet: non null and points to enough memory for the ipv4 structure.
 * Returns zero if parsing was successful. packet will be initialized correctly.
 */
int parse_ipv4_packet(const u_char *data, struct pcap_pkthdr *pkt_header,
                      struct ipv4_packet *packet) {
  packet->header = (const struct ipv4_header *)data;
  assert(packet->header->version_and_ihl >> 4 == 4); // version == IPV4
  assert((packet->header->version_and_ihl & 0x0f) ==
         5); // IHL == 5 (no options supported)

  packet->data = data + 20;
  packet->data_len = pkt_header->len;
  return 0;
}

int main(int argc, char **argv) {
  char errbuf[PCAP_ERRBUF_SIZE];
  bzero(errbuf, PCAP_ERRBUF_SIZE);

  if (argc != 2) {
    fprintf(stderr, "Usage: ./packet_sniffer if\n");
    exit(1);
  }

  pcap_t *sniff = pcap_create(argv[1], errbuf);
  if (!sniff) {
    fprintf(stderr, "pcap_create failed: %s", errbuf);
    exit(1);
  }

  // pcap options
  if (pcap_set_timeout(sniff, 1000) < 0) {
    fprintf(stderr, "failed to set timeout on pcap handle");
    exit(1);
  }

  if (pcap_set_snaplen(sniff, 65535) < 0) {
    fprintf(stderr, "failed to set snaplen on pcap handle");
    exit(1);
  }

  if (pcap_set_promisc(sniff, 0) < 0) {
    fprintf(stderr, "failed to unset promiscuous mode");
    exit(1);
  }

  int rv = pcap_activate(sniff);
  if (rv != 0) {
    fprintf(stderr, "pcap_activate returned non-zero: %d", rv);
  }

  int link_layer_type = pcap_datalink(sniff);
  fprintf(stderr, "link layer header type: %d", link_layer_type);

  for (int i = 0; i < 100; ++i) {
    fprintf(stderr, "reading...\n");
    struct pcap_pkthdr pkt_header;
    const u_char *data = pcap_next(sniff, &pkt_header);

    if (data) {
      fprintf(stderr, "data is nonzero!\n");
      struct ipv4_packet packet;
      if (parse_ipv4_packet((void *)(data + 4), &pkt_header, &packet)) {
        fprintf(stderr, "parsing failed!");
      }
      fprintf(stderr, "Header: %lx", (uintptr_t)&packet);
    }
  }
}
