/* RAFAEL RUBIM CABRAL 1511068 3WA */
/* LIVIA ALOISE 1510952 3WA */

#include "int128.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	
	Int128 a, b, t;
	long c = 0x7654321234567891;
	long d = 0x8abcdefedcbabcd0;
	int i;
	FILE* arq;
	
	//Testes de attr:
	puts("Teste attr:\n");
	
	int128_attr(&a, c);
	int128_attr(&b, d);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	
	
	//Testes de shift:
	
	//Shift Left
	puts("\nTeste shl:\n");
	
	int128_shl(&t, &a, 0);
	int128_debugDump("t=a<<0", &t);
	
	for (i = 0; i < 128/4; i++) {
		int128_shl(&t, &t, 4);
		int128_debugDump("t<<=4", &t);
	}
	
	int128_shl(&t, &a, 0);
	int128_debugDump("t=a<<0", &t);
	int128_shl(&t, &t, 64);
	int128_debugDump("t<<=64", &t);
	int128_shl(&t, &t, 64);
	int128_debugDump("t<<=64", &t);
	
	int128_shl(&t, &a, 0);
	int128_debugDump("t=a<<0", &t);
	int128_shl(&t, &t, 127);
	int128_debugDump("t<<=127", &t);
	int128_shl(&t, &b, 0);
	int128_debugDump("t=b<<0", &t);
	int128_shl(&t, &t, 127);
	int128_debugDump("t<<=127", &t);
	
	for (i = 0; i < 128/4; i++) {
		int128_shl(&t, &a, i*4);
		int128_debugDump("t=a<<i*4", &t);
	}
	
	int128_shl(&t, &a, 64);
	int128_debugDump("t=a<<64", &t);
	
	int128_shl(&t, &a, 127);
	int128_debugDump("t=a<<127", &t);
	int128_shl(&t, &b, 127);
	int128_debugDump("t=b<<127", &t);
	
	// Shift Right
	puts("\nTeste shr:\n");
	
	int128_shl(&t, &a, 64);
	int128_debugDump("t=a<<64", &t);
	
	for (i = 0; i < 128/4; i++) {
		int128_shr(&t, &t, 4);
		int128_debugDump("t>>=4", &t);
	}
	
	int128_shl(&t, &b, 64);
	int128_debugDump("t=b<<64", &t);
	
	for (i = 0; i < 128/4; i++) {
		int128_shr(&t, &t, 4);
		int128_debugDump("t>>=4", &t);
	}
	
	int128_shl(&t, &a, 64);
	int128_debugDump("t=a<<64", &t);
	int128_shr(&t, &t, 64);
	int128_debugDump("t>>=64", &t);
	int128_shr(&t, &t, 64);
	int128_debugDump("t>>=64", &t);
	
	int128_shl(&t, &b, 64);
	int128_debugDump("t=b<<64", &t);
	int128_shr(&t, &t, 64);
	int128_debugDump("t>>=64", &t);
	int128_shr(&t, &t, 64);
	int128_debugDump("t>>=64", &t);
	
	int128_shl(&t, &a, 64);
	int128_debugDump("t=a<<64", &t);
	int128_shr(&t, &t, 127);
	int128_debugDump("t>>=127", &t);
	int128_shl(&t, &b, 64);
	int128_debugDump("t=b<<64", &t);
	int128_shr(&t, &t, 127);
	int128_debugDump("t>>=127", &t);
	
	for (i = 0; i < 128/4; i++) {
		int128_shl(&t, &a, 64);
		int128_shr(&t, &t, i*4);
		int128_debugDump("t=(a<<64)>>i*4", &t);
	}
	
	for (i = 0; i < 128/4; i++) {
		int128_shl(&t, &b, 64);
		int128_shr(&t, &t, i*4);
		int128_debugDump("t=(b<<64)>>i*4", &t);
	}
	
	int128_shl(&t, &a, 64);
	int128_shr(&t, &t, 64);
	int128_debugDump("t=(a<<64)>>64", &t);
	
	int128_shl(&t, &b, 64);
	int128_shr(&t, &t, 64);
	int128_debugDump("t=(b<<64)>>64", &t);
	
	int128_shl(&t, &a, 64);
	int128_shr(&t, &t, 127);
	int128_debugDump("t=(a<<64)>>127", &t);
	int128_shl(&t, &b, 64);
	int128_shr(&t, &t, 127);
	int128_debugDump("t=(b<<64)>>127", &t);
	
	
	
	// Testes de write/read:
	puts("\nTeste write/read:\n");
	
	arq = fopen("int128.data", "wb");
	if (!arq)
		{printf("Erro(1) em criacao de arquivo.\n\n");	exit(1);}
	int128_debugDump("escrevendo a1 no arquivo", &a);
	int128_debugDump("escrevendo b1 no arquivo", &b);
	int128_write(&a, arq);
	int128_write(&b, arq);
	int128_shl(&a, &a, 64);
	int128_shl(&b, &b, 64);
	int128_debugDump("escrevendo a2 no arquivo", &a);
	int128_debugDump("escrevendo b2 no arquivo", &b);
	int128_write(&a, arq);
	int128_write(&b, arq);
	
	fclose(arq);
	arq = fopen("int128.data", "rb");
	if (!arq)
		{printf("Erro(2) em leitura de arquivo.\n\n");	exit(1);}
	if (int128_read(&b, arq) == -1)
		{puts("b1 nao lido");}
	if (int128_read(&a, arq) == -1)
		{puts("a1 nao lido");}
	int128_debugDump("a1, segunda leitura", &a);
	int128_debugDump("b1, primeira leitura", &b);
	if (int128_read(&b, arq) == -1)
		{puts("b2 nao lido");}
	if (int128_read(&a, arq) == -1)
		{puts("a2 nao lido");}
	int128_debugDump("a2, quarta leitura", &a);
	int128_debugDump("b2, terceira leitura", &b);
	if (int128_read(&t, arq) == -1)
		{puts("t nao lido");}
	fclose(arq);
	
	
	// Testes de soma/subtração:
	
	// Testes de soma:
	puts("\nTeste soma:\n");
	
	c = 0x0000000100000000;
	d = 0x0000000000000001;
	int128_attr(&a, c);
	int128_attr(&b, c);
	int128_add(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a+b", &t);
	int128_attr(&a, c);
	int128_shl(&a, &a, 64);
	int128_attr(&b, c);
	int128_shl(&b, &b, 64);
	int128_add(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a+b", &t);
	int128_attr(&a, d);
	int128_shl(&a, &a, 63);
	int128_attr(&b, d);
	int128_shl(&b, &b, 63);
	int128_add(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a+b", &t);
	int128_attr(&a, d);
	int128_shl(&a, &a, 127);
	int128_attr(&b, d);
	int128_shl(&b, &b, 127);
	int128_add(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a+b", &t);
	
	// Testes de subtração:
	puts("\nTeste subtracao:\n");
	
	c = 0x0000000100000000;
	d = 0x0000000000000001;
	int128_attr(&a, c);
	int128_attr(&b, c);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_attr(&a, c);
	int128_shl(&a, &a, 64);
	int128_attr(&b, c);
	int128_shl(&b, &b, 64);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_attr(&a, 0);
	int128_attr(&b, d);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_attr(&a, 0);
	int128_attr(&b, d);
	int128_shl(&b, &b, 12);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_attr(&a, 0);
	int128_attr(&b, d);
	int128_shl(&b, &b, 76);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_attr(&a, 0);
	int128_attr(&b, d);
	int128_shl(&b, &b, 63);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	c = 0xffffffffffffffff;
	d = 0x0f0f0f0f0f0f0f0f;
	int128_attr(&a, c);
	int128_attr(&b, c);
	int128_attr(&b, d);
	int128_attr(&t, d);
	int128_shl(&b, &b, 64);
	int128_add(&b, &b, &t);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	int128_shl(&b, &b, 4);
	int128_sub(&t, &a, &b);
	int128_debugDump("a", &a);
	int128_debugDump("b", &b);
	int128_debugDump("a-b", &t);
	
	return 0;
}
