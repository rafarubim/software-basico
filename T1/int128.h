#include <stdio.h>

typedef struct int128 {
  long high; /* parte alta do valor:  bits 127 a 64 */
  long low ; /* parte baixa do valor: bits 63 a 0   */
} Int128;

/* Atribui��o */
void int128_attr (Int128 *res, long l);

/* Soma */
void int128_add (Int128 *res, Int128 *v1, Int128 *v2);

/* Substra��o */
void int128_sub (Int128 *res, Int128 *v1, Int128 *v2);

/* Shift para esquerda */
void int128_shl (Int128 *res, Int128 *v, int n);

/* Shift para direita (aritmetico) */
void int128_shr (Int128 *res, Int128 *v, int n);

/* Grava um Int128 */
int int128_write(Int128 *v, FILE *f);

/* L� um Int128 */
int int128_read(Int128 *v, FILE *f);

/* Imprime o Int128 na mem�ria */
void int128_debugDump(char* nome, Int128 *v);
