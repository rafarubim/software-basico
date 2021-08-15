#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

struct X1 {
  char c1;
  int i;
  char c2;
} x1 = {0xa1, 0xb1b2b3b4, 0xc1};

struct X2 {
  long l;
  char c;
} x2 = {0xa1a2a3a4a5a6a7a8, 0xb1};

struct X3 {
  int i;
  char c1;
  char c2;
} x3 = {0xa1a2a3a4, 0xb1, 0xc1};

struct X4 {
  struct X2 x;
  char c;
} x4 = {{0xa1a2a3a4a5a6a7a8, 0xb1}, 0xc1};

struct X5 {
  char c1;
  char c2;
  char c3;
} x5 = {0xa1, 0xa2, 0xa3};

struct X6 {
  short s1;
  int i;
  char c[3];
  short s2;
} x6 = {0xa1a2, 0xb1b2b3b4, {0xc1,0xc2,0xc3}, 0xd1d2};

union U1 {
  int i;
  char c[5];
} u1;

union U2 {
  short s;
  char c[5];
} u2;

int main() {
  
  int i;
  for (i = 0; i < 5; i++) {
    u1.c[i] = i;
    u2.c[i] = i;
  }
  
  printf("Dump de x1: \n");
  dump(&x1, sizeof(x1));
  
  printf("\nDump de x2: \n");
  dump(&x2, sizeof(x2));
  
  printf("\nDump de x3: \n");
  dump(&x3, sizeof(x3));
  
  printf("\nDump de x4: \n");
  dump(&x4, sizeof(x4));
  
  printf("\nDump de x5: \n");
  dump(&x5, sizeof(x5));
  
  printf("\nDump de x6: \n");
  dump(&x6, sizeof(x6));
  
  printf("\nDump de u1: \n");
  dump(&u1, sizeof(u1));
  
  printf("\nDump de u2: \n");
  dump(&u2, sizeof(u2));
  return 0;
}
