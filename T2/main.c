#include <stdio.h>
#include <stdlib.h>
#include "geracod.h"

typedef int (*funcp) (int x);

int main(int argc, char** argv) {
	void* codigoSBF;
	funcp mainSBF;
	int paramSBF, retSBF;
	FILE* arqSBF;
	if (argc != 3)	{printf("Uso do programa: %s {\"PATH_SBF\"} {PARAM}\n", argv[0]);	exit(1);}
	paramSBF = atoi(argv[2]);
	arqSBF = fopen(argv[1],"r");
	while(!arqSBF) {
		char novoPath[101];
		printf("Path para arquivo SBF inválido. Digite um novo path: ");
		scanf(" %[^\n]", novoPath);
		arqSBF = fopen(novoPath,"r");
	}
	geracod(arqSBF, &codigoSBF, &mainSBF);
	retSBF = mainSBF(paramSBF);
	printf("Retorno do programa: %d\n\n", retSBF);
	liberacod(codigoSBF);
	return 0;
}
