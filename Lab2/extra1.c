#include <stdio.h>

int is_little() {
	int a = 1;
	char *b = (char*) &a;
	return *b;
}

int main() {
	char a = is_little();
	if (a)
		printf("É little-endian!\n");
	else
		printf("Não é little-endian!\n");
	return 0;
}
