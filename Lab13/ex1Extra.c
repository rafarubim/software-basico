#include <stdio.h>
#include <math.h>
#include <limits.h>

#define getsig(x)        ((x)>>31 & 1)
#define getexp(x)        ((x)>>23 & 0xff)
#define getfrac(x)       ((x) & 0x7fffff)
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))

typedef union {
  float f;
  unsigned int i;
} U; 
   
int float2int(float f) {
	U u;
	unsigned int sig, exp, frac;
	int inteiro, pot;
	u.f = f;
	sig = getsig(u.i);
	exp = getexp(u.i);
	frac = getfrac(u.i);
	pot = exp-127;
	if (sig == 0 && exp == 0 && frac == 0)
		return 0;
	if (pot <= 23)
		inteiro = (frac | 0x800000) >> (23-pot);
	else if (pot <= 29)
		inteiro = (frac | 0x800000) << (pot-23);
	else /* Não cabe no inteiro */
			 if (sig)
		return INT_MIN;
	else
		return INT_MAX;
	if (sig)
		inteiro = -inteiro;
	return inteiro;
}

int main() {
   float f;

   printf("\n******** float2int ****************\n");
   f = 0.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 1.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = -1.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 12345.0;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = -12345.0;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 1.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 2.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 2.4;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 0.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = (float) pow(2,31);
   printf(" %+10.4f (2^31) = overflow -> %d\n", f, float2int(f));
   f = -f;
   printf(" %+10.4f (-2^31) -> %d\n", f, float2int(f));
   f = (float) pow(2,32);
   f = -f;
   printf(" %+10.4f (-2^32 = overflow) -> %d\n", f, float2int(f));

   return 0;
}

