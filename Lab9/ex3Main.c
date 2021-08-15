#include <stdio.h>

void foo (int a[], int n);

int main() {
	int i, a[] = {1, 2, 0, 3, 5, 0};
	foo(a, 6); // Resultado esperado: {1, 2, 3, 4, 5, 8}
	for (i=0; i<6;i++)
		printf("%d\n", a[i]);
		return 0;
}
