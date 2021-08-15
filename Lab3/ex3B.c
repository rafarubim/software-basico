#include <stdio.h>

unsigned char rotate_left(unsigned char x, int n) {
	unsigned char a = x;
	n %= 8;
	return (a << n) | (x >> (8-n));
}

int main() {
	printf("%x\n", rotate_left(0b00010101, 1));
	printf("%x\n", rotate_left(0x61, 2));
	printf("%x\n", rotate_left(0x61, 7));
	return 0;
}
