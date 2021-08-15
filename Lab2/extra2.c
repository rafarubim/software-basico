#include <stdio.h>

void num2string(char *s, int num, int base) {
	int a = 0;
	int b;
	int c = 1;
	int d = 1;
	
	while (1) {
		
		b = (num % (10*c)) - (num % c);
		b /= c
		a = (a) * base;
		
		c *= 10;
		d++;
	}
}
