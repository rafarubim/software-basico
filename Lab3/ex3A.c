#include <stdio.h>

unsigned char switch_byte(unsigned char x) {
	unsigned char a = x;
	return (a << 4) | (x >> 4);
}

int main() {
  printf("%x\n", switch_byte(0xAB));
  printf("%x\n", switch_byte(0x12));
  return 0;
}
