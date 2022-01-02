#include <stdint.h>
#include <stdio.h>

union Data {
  uint32_t integer;
  uint8_t data[4];
};

union Flags {
  uint8_t flag;
  struct {
    int bit_1 : 1;
    int bit_2 : 1;
    int bit_3 : 1;
    int bit_4 : 1;
    int bit_5 : 1;
    int bit_6 : 1;
    int bit_7 : 1;
    int bit_8 : 1;
  };
};

int main(void) {
  union Data d;
  fprintf(stderr, "size of union: %lu\n", sizeof(d));

  d.integer = 0x000000ff;
  for (int i = 0; i < 4; ++i) {
    fprintf(stderr, "d[%d] = %u\n", i, d.data[i]);
  }

  union Flags f = { 0 };
  f.bit_1 = 1;
  fprintf(stderr, "flag: %08x", f.flag);
}
