#include <stdio.h>

#define getdoublesig(x)     ((x)>>63 & 1)
#define getdoubleexp(x)     ((x)>>52 & 0x7ff)
#define getdoublefrac(x)    ((x) & 0xfffffffffffff)
#define makedouble(s,e,f) 	((s & 1)<<63 | (((e) & 0x7ff) << 52) | ((f) & 0xfffffffffffff))

typedef union {
  double d;
  unsigned long l;
} U; 

double maxdouble(double a, double b) {
	U ua, ub;
	unsigned long sigA, expA, fracA, sigB, expB, fracB;
	ua.d = a;
	ub.d = b;
	sigA = getdoublesig(ua.l);
	expA = getdoubleexp(ua.l);
	fracA = getdoublefrac(ua.l);
	sigB = getdoublesig(ub.l);
	expB = getdoubleexp(ub.l);
	fracB = getdoublefrac(ub.l);
	if (sigA > sigB)
		return b;
	else if (sigB > sigA)
		return a;
	if (expA > expB)
		if (!sigA)
			return a;
		else
			return b;
	else if (expB > expA)
		if (!sigA)
			return b;
		else
			return a;
	if (fracA > fracB)
		if (!sigA)
			return a;
		else
			return b;
	else
		if (!sigA)
			return b;
		else
			return a;
}

int main() {
   double a,b;

   a = 1.0;
   b = 2.0;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.0;
   b = -2.0;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.5;
   b = 1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = -1.5;
   b = -1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.5;
   b = -1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   return 0;
}

