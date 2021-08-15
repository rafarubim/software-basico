#include <stdio.h>
#include <stdlib.h>

double foo3 (double *a, int n);

double vec[] = {3.14, 1.57, 0, 3.14, 1.57, 4.71};

int main() {
	
	printf("foo3(vec, 6) = %f\n", foo3(vec, 6));
	
	return 0;
}
