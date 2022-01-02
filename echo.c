#include <libnet.h>
#include <pcap/pcap.h>

int main(void) {
  char errbuf[LIBNET_ERRBUF_SIZE];
  libnet_t *l = libnet_init(LIBNET_RAW4, "en0", errbuf);
  if (!l) {
    fprintf(stderr, "libnet_init failed: %s", errbuf);
  }

  libnet_ptag_t icmp_tag = libnet_build_icmpv4_echo(ICMP_ECHO, 0, 0, 1, 1, NULL, 0, l, 0);
  if (icmp_tag == -1) {
    fprintf(stderr, "icmp packet build failed: %s", libnet_geterror(l));
  }


  uint32_t addr = 0x000000ff; // loopback
  libnet_ptag_t ip_tag = libnet_autobuild_ipv4(20+8, IPPROTO_ICMP, addr, l);
  if (ip_tag == -1) {
    fprintf(stderr, "ipv4 packet build failed: %s", libnet_geterror(l));
  }

  if (libnet_write(l) == -1) {
    fprintf(stderr, "libnet write failed with: %s", libnet_geterror(l));
  }
  libnet_destroy(l); 
}
