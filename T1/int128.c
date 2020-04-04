/* RAFAEL RUBIM CABRAL 1511068 3WA */
/* LIVIA ALOISE 1510952 3WA */

#include "int128.h"

#include <stdio.h>

/* Atribuição */
void int128_attr (Int128 *res, long l) {

	res->low = l;
	if (l < 0)
		res->high = 0xffffffffffffffff;
	else
		res->high = 0;
}

/* Soma */
void int128_add (Int128 *res, Int128 *v1, Int128 *v2) {
	unsigned long temp = v1->low; // caso res = v1 ou v2, impede bug
	res->low = v1->low + v2->low;
	if (temp <= res->low) // não deu overflow no low
		res->high = v1->high + v2->high;
	else // deu overflow no low
		res->high = v1->high + v2->high + 1;
}

/* Substração */
void int128_sub (Int128 *res, Int128 *v1, Int128 *v2) {
	unsigned long temp = v1->low; // caso res = v1 ou v2, impede bug
	long complLow = v2->low ^ 0xffffffffffffffff;
	long complHigh = v2->high ^ 0xffffffffffffffff;
	res->low = v1->low + complLow + 1;
	if (temp <= res->low-1 /*teste de overflow na soma (v1->low + complLow)*/&& (unsigned long)1 <= res->low/*teste de overflow na soma (v1->low + complLow)+ 1*/) // não deu overflow no low
		res->high = v1->high + complHigh;
	else // deu overflow no low
		res->high = v1->high + complHigh + 1;
}

/* Shift para esquerda */
void int128_shl (Int128 *res, Int128 *v, int n) {
	int desloc;
	unsigned long save;
	if (n >= 64) {
		desloc = 0;
		save =  -1;
	} else {
		desloc = 64 - n;
		save = (((long) 1) << n) - 1;
	}
	save <<= desloc;
	save &= v->low;
	save >>= desloc;
	if (n < 64) {
		res->low = (v->low << n);
		res->high = (v->high << n);
		res->high |= save;
	} else {
		res->low = 0;
		res->high = (save << (n - 64));
	}
}

/* Shift para direita (aritmetico) */
void int128_shr (Int128 *res, Int128 *v, int n) {
	int desloc;
	long save;
	if (n >= 64) {
		desloc = 0;
		save =  -1;
	} else {
		desloc = 64 - n;
		save = (((long) 1) << n) - 1;
	}
	save &= v->high;
	save <<= desloc;
	if (n < 64) {
		res->high = (v->high >> n);
		res->low = (((unsigned long)v->low) >> n);
		res->low |= save;
	} else {
		if (v->high >= 0)
			res->high = 0;
		else
			res->high = -1;
		res->low = (save >> (n - 64));
	}
}

int int128_write(Int128 *v, FILE *f) {
	int i;
	for(i = 7; i >= 0; i--)
		if (fwrite(((char*)(&v->high))+i, 1, 1, f) != 1)
			return -1;
	for(i = 7; i >= 0; i--)
		if (fwrite(((char*)(&v->low))+i, 1, 1, f) != 1)
			return -1;
	return 0;
}

int int128_read(Int128 *v, FILE *f) {
	int i;
	for(i = 7; i >= 0; i--)
		if (fread(((char*)(&v->high))+i, 1, 1, f) != 1)
			return -1;
	for(i = 7; i >= 0; i--)
		if (fread(((char*)(&v->low))+i, 1, 1, f) != 1)
			return -1;
	return 0;
}

void int128_debugDump(char* nome, Int128 *v) {

	printf("%s:\t%016lx%016lx\n", nome, v->high, v->low);
}
