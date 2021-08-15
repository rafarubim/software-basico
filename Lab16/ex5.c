#include <stdio.h>
unsigned char codigo[] = {0x55,0x48,0x89,0xe5,0xe8,0x00,0x00,0x00,0x00,0xc9,0xc3};

typedef int (*funcp) (int x);

int add (int x) {
  return x+1;
}

unsigned char getByte(int byte, int var32bits) { /* devolve um dos bytes (0 a 3) de um int */
	unsigned int tool = 0xff << 8*byte;
	tool &= var32bits;
	tool >>= 8*byte;
	return (unsigned char)tool;
}

int main() {
	int i, diferenca;
	funcp foo = (funcp)codigo;
	diferenca = (int) ((unsigned long)add - (unsigned long)&codigo[9]);
	for (i=0;i<4;i++)
		codigo[5+i] = getByte(i,diferenca);
	printf("foo(10) = %d\n",foo(10));
	return 0;
}
