#include <stdio.h>

int odd_ones(unsigned int x) {
  
  int a = 0, i;
  for(i = 0; i < 8*sizeof(unsigned int); i++)
    if ((1 << i) & x)
      a = !a;
  return a;
}

int main() {
  printf("%x tem numero %s de bits\n",0x01010101,odd_ones(0x01010101) ? "impar":"par");
  printf("%x tem numero %s de bits\n",0x01030101,odd_ones(0x01030101) ? "impar":"par");
  return 0;
}
