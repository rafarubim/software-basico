#include <stdio.h>
#include <stdlib.h>

double foo2 (float a, float b);

int main(int argc, char** argv) {
	float a, b;
	if (argc != 3)
		{printf("Uso do programa: %s -a -b\n",argv[0]);	exit(1);}
	a = (float) atof(argv[1]);
	b = (float) atof(argv[2]);
	printf("foo2(%.2f,%.2f): %f\n", a, b, foo2(a,b));
	return 0;
}
