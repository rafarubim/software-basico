#include <stdio.h>

typedef int (*funcp) (int x);

void geracod (FILE *f, void **code, funcp *entry);
void liberacod (void* p);
