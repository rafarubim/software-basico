#include <ctype.h>
#include <stdio.h>

int string2num (char *s, int base) {
  int a = 0;
  for (; *s; s++)
    if (*s >= '0' && *s <= '9')
      a = a*base + (*s - '0');
    else if (*s >= 'a' && *s <= 'z')
      a = a*base + 10 + (*s - 'a');
    else if (*s >= 'A' && *s <= 'Z')
      a = a*base + 10 + (*s - 'Z');
  return a;
}

int main (void) {
  printf("%d\n", string2num("1a", 16));
  printf("%d\n", string2num("a09b", 16));
  printf("%d\n", string2num("z09b", 36));
  return 0;
}
