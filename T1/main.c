#include "int128.h"
#include <stdio.h>

int main() {
	
	Int128 a, c;
	int i;
	long d = 0x123456787654321a;
	
	int128_attr(&a, d);
	puts("");
	int128_shl(&c, &a, 0);
	for (i = 0; i < (128/4); i++) {
		int128_shl(&c, &c, 4);
		int128_debugPrint(&c);
		puts("");
	}
	
	return 0;
}
