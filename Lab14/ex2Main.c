#include <stdio.h>
#include <stdlib.h>

float foo1 (float a, float b);

int main(int argc, char** argv) {
	float a, b;
	if (argc != 3)
		{printf("Uso do programa: %s -a -b\n", argv[0]);	exit(1);}
	a = (float) atof(argv[1]);
	b = (float) atof(argv[2]);
	printf("foo1(%.2f, %.2f) = %f\n", a, b, foo1(a, b));
	return 0;
}
