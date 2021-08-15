#include <stdio.h>

int fat(int n);

int main() {
	int i;
	for (i=0; i<=5; i++)
		printf("%d\n", fat(i));
	return 0;
}
