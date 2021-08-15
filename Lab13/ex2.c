#include "stdio.h"

#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define getsig(x)  ((x)>>31 & 1)
#define getexp(x)  ((x)>>23 & 0xff)
#define getfrac(x) ((x) & 0x7fffff)

typedef union { 
  float f;
  unsigned int i;
 } U;

float int2float(int a) {
	unsigned int sig, exp, frac;
	int count = 0;
	U u;
	if (a == 0) {
		sig = 0;
		exp = 0;
		frac = 0;
	} else {
		if (a > 0)
			sig = 0;
		else {
			sig = 1;
			a = -a;
		}
		while(a > 0) {
			a <<= 1;
			count++;
		}
		exp = 32-count-1;
		exp += 127;
		frac = a << 1;
		frac >>= 9;
	}
	u.i = makefloat(sig,exp,frac);
	return u.f;
}

int main() {
   int i;

   printf("\n******** int2float ****************\n");
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float(i));
   i = 0x7fffffff;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   return 0;
}

