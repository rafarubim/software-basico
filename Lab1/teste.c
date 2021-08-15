#include <stdio.h>
#include <stdlib.h>

void dump (void *p, int n);

int main(int argc, char** argv) {

  if (argc != 2) {
    printf("Número incorreto de parâmetros para \"%s\". Parâmetros dados: %d (espera-se 1)", argv[0], argc - 1);
    exit(0);
  }
		
  char c = 100;
  int i = atof(argv[1]);
  char v[] = "abcde";

  printf("valor de c: %d na memória: ",c);
  dump(&c, sizeof(c));

  printf("\nvalor de i: %d na memória: ",i);
  dump(&i, sizeof(i));

  printf("\nvalor de v: %s na memória: ",v);
  dump(v, sizeof(v));

  printf("\n");
  return 0;
}
