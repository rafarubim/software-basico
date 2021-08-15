#include <stdio.h>
unsigned char codigo[] = {0x55,0x48,0x89,0xe5,0x89,0xf8,0x83,0xc0,0x01,0xc9,0xc3};

typedef int (*funcp) (int x);

int main() {
	int i;
	funcp f = (funcp)codigo;
	i = f(10);
	printf("foo(10) = %d\n",i);
	return 0;
}
