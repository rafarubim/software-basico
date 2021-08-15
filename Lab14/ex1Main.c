#include <stdio.h>
#include <stdlib.h>

float foo (double a, float b);

int main(int argc, char** argv) {
	double a;
	float b;
	if (argc != 3)
		{printf("Uso do programa: %s -a -b\n", argv[0]);	exit(1);}
	a = atof(argv[1]);
	b = (float) atof(argv[2]);
	printf("foo(%.2f, %.2f) = %f\n", a, b, foo(a,b));
	return 0;
}
