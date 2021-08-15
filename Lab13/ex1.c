#include "stdio.h"

#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define getsig(x)  ((x)>>31 & 1)
#define getexp(x)  ((x)>>23 & 0xff)
#define getfrac(x) ((x) & 0x7fffff)

typedef union { 
  float f;
  unsigned int i;
 } U;

float float2(float f) {
	U u;
	unsigned int i, sig, exp, frac;
	u.f = f;
	i = u.i;
	sig = getsig(i);
	exp = getexp(i);
	frac = getfrac(i);
	exp += 1;
	u.i = makefloat(sig,exp,frac);
	return u.f;
}

int main() {
	float f = 1235.9876;
	f = float2(f);
	printf("%.4f\n", f);
}
