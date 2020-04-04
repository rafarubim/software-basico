/* Rafael Rubim Cabral 	- 1511068 - 3WA */
/* Lívia Aloise					- 1510952 - 3WA */

#include <stdlib.h>
#include <string.h>
#include "geracod.h"

/*********************************************************************************************
*		Tipo enumerado:		VPC
*		Descrição:
*			Representa uma estrutura da linguagem SBF. As 3 estruturas possíveis são: variável,
*			parâmetro e constante.
*		Símbolos:
*			VPC_NONE:		Significa que não é uma estrutura SBF (nenhuma das possibilidades abaixo) ou
*									é um número que não é precedido de caracter, usado para chamar funções.
*			VPC_VAR:		Representa variável SBF.
*			VPC_PARM:		Representa parâmetro SBF.
*			VPC_CONST:	Representa constante SBF.
*********************************************************************************************/
typedef enum {
	VPC_NONE = 0,
	VPC_VAR,
	VPC_PARM,
	VPC_CONST
} VPC; /* Variável/Parâmetro/Constante */

/*********************************************************************************************
*		Tipo enumerado:		Operador
*		Descrição:
*			Representa um operador da linguagem SBF.
*		Símbolos:
*			OPR_NONE:		Significa que não é um operador SBF (nenhuma das possibilidades abaixo).
*			OPR_SUM:		Representa o operador SBF "+".
*			OPR_SUB:		Representa o operador SBF "-".
*			OPR_MULT:		Representa o operador SBF "*".
*********************************************************************************************/
typedef enum {
	OPR_NONE = 0,
	OPR_SUM,
	OPR_SUB,
	OPR_MULT
} Operador;

/*********************************************************************************************
*		Tipo enumerado:		Comando
*		Descrição:
*			Representa uma linha de comando da linguagem SBF.
*		Símbolos:
*			CMD_FUNC:	Representa o comando "function".
*			CMD_END:	Representa o comando "end".
*			CMD_RET:	Representa o comando "ret?" (Comando com 2 "parâmetros" A, B. Ex: "ret? A B").
*			CMD_CALL:	Representa o comando de atribuição com chamada de função "= call" (Comando com
*								3 "parâmetros" A, B, C. Ex: A = call B C, onde A é uma variável e B um inteiro).
*			CMD_OPR:	Representa o comando de atribuição com operação (Comando com 3 "parâmetros"
*								A, B, C e um operador OPR. Ex: A = B OPR C, onde A é uma variável e OPR é um
*								operador).
*********************************************************************************************/
typedef enum {
	CMD_FUNC,
	CMD_END,
	CMD_RET,
	CMD_CALL,
	CMD_OPR,
} Comando;

/* Alocar: 50MB */
/* UNIDADE_ALOCACAO: Quantidade de bytes da unidade escolhida (1000 -> 1KB, 1000000 -> 1MB, ...) */
#define UNIDADE_ALOCACAO 1000000
/* ALOCACAO_MAX: Tamanho do espaço alocado para o código SBF, na unidade escolhida */
#define ALOCACAO_MAX 50
/* TAMANHO_CODIGO: Tamanho, em bytes, do espaço alocado para código SBF */
#define TAMANHO_CODIGO (UNIDADE_ALOCACAO * ALOCACAO_MAX)

/* MAX_FUNCS: Quantidade máxima de funções permitidas num código SBF */
#define MAX_FUNCS 20
/* MAX_LOCAL_VARS: Quantidade máxima de variáveis locais (só serve a propósito de interpretação de código) */
#define MAX_LOCAL_VARS 5
/* NUM_PARAMS: Quantidade de parâmetros das funções (só serve a propósito de interpretação de código) */
#define NUM_PARAMS 1

/*********************************************************************************************
*		Função:			error
*		Descrição:
*			Apresenta mensagem de erro sobre codigo SBF e aborta programa.
*		Parâmetros:
*			msg:	Mensagem de erro.
*			line:	Linha do arquivo SBF em que erro ocorreu, a ser especificada na mensagem de erro.
*********************************************************************************************/
static void error (const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

/*********************************************************************************************
*		Função:			isBlank
*		Descrição:
*			Descobre se caracter é branco, ou seja, é aceito após um comando SBF. Ex: "function&" não
*			é um comando SBF, pois é o comando SBF "function" seguido do caracter não branco "&".
*		Parâmetros:
*			c:	Caracter a ser avaliado.
*		Retorno:
*			Retorna 1 caso caracter seja branco, 0 caso contrário.
*********************************************************************************************/
static int isBlank(char c) {
	if (c == ' ')
		return 1;
	else
		return 0;
}

/*********************************************************************************************
*		Função:			getOpr
*		Descrição:
*			Descobre o tipo de operador que um caracter representa.
*		Parâmetros:
*			c:	Caracter a ser avaliado.
*		Retorno:
*			Retorna qualquer constante do tipo enumerado "Operador". Retorna OPR_NONE caso o caracter
*			não represente um operador.
*********************************************************************************************/
static Operador getOpr(char c) {
	if (c == '+')
		return OPR_SUM;
	if (c == '-')
		return OPR_SUB;
	if (c == '*')
		return OPR_MULT;
	return OPR_NONE;
}

/*********************************************************************************************
*		Função:			getVPC
*		Descrição:
*			Descobre o tipo de estrutura SBF que um caracter seguido de um número representa. Ex: $4
*			representa uma constante SBF. A função leva em conta os índices máximos de variáveis e
*			parâmetros SBF. Ex: v6 não é uma variável SBF caso o número máximo de variáveis locais de
*			uma função SBF seja 5. Tais limites para índices são descritos pelas constantes
*			MAX_LOCAL_VARS e NUM_PARAMS.
*		Parâmetros:
*			c:		Caracter da estrutura.
*			num:	Número da estrutura.
*		Retorno:
*			Retorna qualquer constante do tipo enumerado "VPC". Retorna VPC_NONE caso o conjunto
*			caracter-número não represente uma estrutura SBF.
*********************************************************************************************/
static VPC getVPC(char c, int num) {
	if (c == 'v' && num >= 0 && num < MAX_LOCAL_VARS)
		return VPC_VAR;
	if (c == 'p' && num >= 0 && num < NUM_PARAMS)
		return VPC_PARM;
	if (c == '$')
		return VPC_CONST;
	return VPC_NONE;
}

/*********************************************************************************************
*		Função:			endCmdLine
*		Descrição:
*			A partir de um arquivo texto já aberto, move seu indicador de posição (usado na leitura
*			do arquivo) para antes do primeiro caracter que não é um espaço ' ' ou uma quebra de linha
*			'\n'. Avalia também se nessa moção foi pulada alguma quebra de linha ou foi atingido o fim
*			do arquivo, com o propósito de saber se um comando SBF foi devidamente encerrado
*			pulando-se uma linha ou se não há mais linhas após ele. 
*			saber 
*		Parâmetros:
*			arqSBF:	Descritor do arquivo texto.
*		Retorno:
*			Retorna 1 caso houve uma quebra de linha ou foi alcançado fim de arquivo. Retorna 0 caso
*			contrário.
*********************************************************************************************/
static int endCmdLine(FILE* arqSBF) {
	int flagLineEnded = 0;
	while(1) {
		char c = fgetc(arqSBF);
		if (c != '\n' && c != ' ' && c != '\t') {
			if (c == EOF)
				return 1;
			if (fseek(arqSBF, -1, SEEK_CUR) != 0)
				return 0;
			break;
		}
		if (c == '\n') {
			flagLineEnded = 1;
		}
	}
	return flagLineEnded;
}

/*********************************************************************************************
*		Função:			colocarCodigo
*		Descrição:
*			Recebe um vetor de bytes (u. char) contendo instruções de máquina e coloca essas
*			instruções no espaço alocado para o código, no primeiro endereço vazio (ou seja, que
*			ainda não possui código). Dá erro e aborta o programa caso as instruções não caibam no
*			espaço alocado para o código. O tamanho de tal espaço é descrito pela constante
*			TAMANHO_CODIGO.
*		Parâmetros:
*			inicioCodigo:			Ponteiro para espaço alocado para o código.
*			enderecoVirtual:	Número que representa, relativamente em bytes, o primeiro endereço
*												vazio (sem instruções) do espaço do código. O número 0 representa o
*												próprio "inicioCodigo", enquanto o 1 representa o próximo byte e assim
*												por diante.
*			vecCode:					Vetor com instruções de máquina a serem anexadas ao código.
*			nBytes:						Tamanho do vetor "vecCode".
*********************************************************************************************/
static void colocarCodigo	(void* inicioCodigo, unsigned long enderecoVirtual,
										unsigned char vecCode[], int nBytes) {
	int i;
	unsigned char* enderecoReal = ((unsigned char*) inicioCodigo) + enderecoVirtual;
	if (enderecoVirtual + (unsigned long) nBytes > TAMANHO_CODIGO)
		{printf("Limite de codigo estourado! Tente alocar mais espaco para o codigo.\n\n");	exit(1);}
	for (i = 0; i < nBytes; i++)
		enderecoReal[i] = vecCode[i];
}

/*********************************************************************************************
*		Função:			alocarTempUC
*		Descrição:
*			Recebe um vetor temporário (um buffer com escopo local, por exemplo) e retorna uma cópia,
*			alocada dinamicamente.
*		Parâmetros:
*			vecTemporario:	Vetor a ser copiado
*			nBytes:					Quantidade de elementos, ou bytes (pois vetor é do tipo u. char) do vetor.
*		Retorno:
*			Retorna o vetor cópia alocado dinamicamente.
*********************************************************************************************/
static unsigned char* alocarTempUC	(unsigned char* vecTemporario, int nBytes) {
	int i;
	unsigned char* vecAlocado = (unsigned char*) malloc(nBytes);
	if (!vecAlocado) {printf("Erro (1): falta de memoria\n\n");	exit(1);}
	for (i = 0; i < nBytes; i++)
		vecAlocado[i] = vecTemporario[i];
	return vecAlocado;
}


/*********************************************************************************************
*		Função:			concatVecUC
*		Descrição:
*			Recebe dois vetores de unsigned char e retorna um vetor alocado dinamicamente que é a
*			concatenação dos dois.
*		Parâmetros:
*			v1:	Primeiro vetor.
*			n1:	Quantidade de elementos do primeiro vetor.
*			v2:	Segundo vetor.
*			n2:	Quantidade de elementos do segundo vetor.
*		Retorno:
*			Retorna o vetor concatenação alocado dinamicamente.
*********************************************************************************************/
static unsigned char* concatVecUC (unsigned char* v1, int n1, unsigned char* v2, int n2) {
	int i;
	unsigned char* vecAlocado = (unsigned char*) malloc(n1+n2);
	if (!vecAlocado) {printf("Erro (5): falta de memoria\n\n");	exit(1);}
	for (i = 0; i < n1; i++)
		vecAlocado[i] = v1[i];
	for (i = 0; i < n2; i++)
		vecAlocado[n1+i] = v2[i];
	return vecAlocado;
}

static char getStackPos (int idxVar) {
	return (-4)*(idxVar+1);
}

/*********************************************************************************************
*		Função:			obterInstrucoes
*		Descrição:
*			Recebe um comando executado em SBF, dois vetores que representam os argumentos do 
*			comando (como documentado na criação do tipo enumerado comando), a quantidade de
*			argumentos do comando, um possível operador usado no comando (ou OPR_NONE), o vetor de
*			posições na memória virtual de funcões já existentes,o número de elementos desse vetor,
*			a posição virtual atual do comando a ser inserido na memória e a linha de código SBF
*			SBF atual (para mensagem de erro). Os dois vetores de argumentos são: um com os tipos
*			dos argumentos (VPC) e outro com os índices dos argumentos. Ex: se os argumentos são
*			 $4 e p0 então os vetores são {VPC_CONST, VPC_PARM} e {4, 0}. Retorna as instruções
*			de máquina correspondentes ao comando SBF num vetor alocado dinamicamente e o tamanho
*			desse vetor.
*		Parâmetros:
*			cmd:				Comando em SBF.
*			nBytesRet:	Referência de inteiro para retorno do tamanho do vetor de instruções
*									retornado.
*		Retorno:
*			Retorna o vetor de instruções de máquina criado.
*********************************************************************************************/
static unsigned char* obterInstrucoes	(Comando cmd, VPC vecTipoParms[], int vecParms[],
																			int qtdParms, Operador opr, unsigned long vecFuncs[],
																	int nFuncs, unsigned long posAtual, int line, int* nBytesRet) {
	unsigned char* vecInstrucoes;
	switch(cmd) {
		case CMD_FUNC: {
			/* pushq %rbp -> 0x55 */
			/* movq %rsp, %rbp -> 0x48 89 e5 */
			/* subq $32, %rsp -> 48 83 ec 20 */
			unsigned char codigo[] =	{0x55,								/* pushq %rbp */
																0x48,0x89,0xe5,				/* movq %rsp, %rbp */
																0x48,0x83,0xec,0x20};	/* subq $32, %rsp */
			*nBytesRet = 8;
			vecInstrucoes = alocarTempUC(codigo,*nBytesRet);
			break;
		}
		case CMD_END: {
			/* leave -> c9 */
			/* ret -> c3 */
			unsigned char codigo[] =	{0xc9,	/* leave */
																0xc3};	/* ret */
			*nBytesRet = 2;
			vecInstrucoes = alocarTempUC(codigo,*nBytesRet);
			break;
		}
		case CMD_RET: {
			unsigned char* compl0;
			int compl0Size;
			unsigned char* jne;
			int jneSize;
			unsigned char* movlEax;
			int movlEaxSize;
			unsigned char* ret;
			int retSize;
			unsigned char* compl0jne;
		
			switch(vecTipoParms[1]) {
				case VPC_VAR: {
					/* movl N(%rbp), %eax -> 8b 45 N */
					char stkPos = getStackPos(vecParms[1]);
					{
						unsigned char codigo[] = {0x8b,0x45,stkPos};
						movlEax = alocarTempUC(codigo,3);
						movlEaxSize = 3;
					}
					break;
				}
				case VPC_PARM: {
					/* movl %edi, %eax -> 89 f8 */
					unsigned char codigo[] = {0x89,0xf8};
					movlEax = alocarTempUC(codigo, 2);
					movlEaxSize = 2;
					break;
				}
				case VPC_CONST: {
					/* movl $0xC, %eax -> 0xb8 CC CC CC CC */
					unsigned char codigo[] = {0xb8};
					movlEax = concatVecUC(codigo,1,(unsigned char*)&vecParms[1],4);
					movlEaxSize = 5;
					break;
				}
				default:
					printf("Erro (6):	inconsistencia em parametro\n\n");	exit(1);
			}
			
			/* leave -> c9 */
			/* ret -> c3 */
			{
				unsigned char codigo[] = {0xc9,0xc3};
				ret = concatVecUC(movlEax,movlEaxSize,codigo,2);
				retSize = movlEaxSize + 2;
				free(movlEax);
			}
		
			switch(vecTipoParms[0]) {
				case VPC_VAR: {
					/* cmpl $0, N(%rbp) ->  83 7d N 00 */
					char stkPos = getStackPos(vecParms[0]); /* ----------------------------------- TESTAR: RET? VX $2 --------------------------------------------*/
					{
						unsigned char codigo[] = {0x83,0x7d,stkPos,0x00};
						compl0 = alocarTempUC(codigo,4);
						compl0Size = 4;
					}
					break;
				}
				case VPC_PARM: {
					/* cmpl $0, %edi -> 83 ff 00 */
					unsigned char codigo[] = {0x83,0xff,0x00};
					compl0 = alocarTempUC(codigo,3);
					compl0Size = 3;
					break;
				}
				case VPC_CONST: {
						/* movl $0x123, %r10d -> 41 ba 23 01 00 00 */
						/* cmpl $0, %r10d -> 41 83 fa 00 */
						unsigned char* movlr10d;
						int movlr10dSize;
						{
							unsigned char codigo[] = {0x41,0xba};
							unsigned char codigo2[] = {0x41,0x83,0xfa,0x00};
							movlr10d = concatVecUC(codigo,2,(unsigned char*)&vecParms[0],4);
							movlr10dSize = 6;
							compl0 = concatVecUC(movlr10d,movlr10dSize,codigo2,4);
							compl0Size = movlr10dSize + 4;
							free(movlr10d);
						}
						
					break;
				}
				
				default:
					printf("Erro (7):	inconsistencia em parametro\n\n");	exit(1);
			}
			
			/* jne N -> 75 N */
			{
				unsigned char codigo[] = {0x75,(unsigned char)retSize};
				jne = alocarTempUC(codigo,2);
				jneSize = 2;
			}
		
			compl0jne = concatVecUC(compl0,compl0Size,jne,jneSize);
			free(compl0);
			free(jne);
			
			*nBytesRet = compl0Size + jneSize + retSize;
			vecInstrucoes = concatVecUC(compl0jne,compl0Size + jneSize,ret,retSize);
			free(compl0jne);
			free(ret);
			break;
		}
		case CMD_CALL: {
			unsigned char* movlEdi;
			int movlEdiSize;
			unsigned char* call;
			int callSize;
			unsigned char* startCall;
			int startCallSize;
			unsigned char* startAndCall;
			int startAndCallSize;
			unsigned char* endCall;
			int endCallSize;
			
			switch(vecTipoParms[2]) {
				case VPC_VAR: {
					/* movl N(%rbp), %edi -> 8b 7d N */
					char stkPos = getStackPos(vecParms[2]);
					{
						unsigned char codigo[] = {0x8b,0x7d,stkPos};
						movlEdi = alocarTempUC(codigo,3);
						movlEdiSize = 3;
					}
					break;
				}
				case VPC_PARM: {
					movlEdi = NULL;
					movlEdiSize = 0;
					break;
				}
				case VPC_CONST: {
					/* movl $0xC, %edi -> bf CC CC CC CC */
					unsigned char codigo[] = {0xbf};
					movlEdi = concatVecUC(codigo,1,(unsigned char*)&vecParms[2],4);
					movlEdiSize = 5;
					break;
				}
				default:
					printf("Erro (8):	inconsistencia em parametro\n\n");	exit(1);
			}
			
			/* movl %edi, -24(%rbp) -> 89 7d e8 */
			{
				unsigned char codigo[] = {0x89,0x7d,0xe8};
				startCall = concatVecUC(codigo,3,movlEdi,movlEdiSize);
				startCallSize = 3 + movlEdiSize;
				free(movlEdi);
			}
			
			/* call N -> e8 NN NN NN NN */
			{
				unsigned char codigo[] = {0xe8};
				int diff;
				if (vecParms[1] >= nFuncs || vecParms[1] < 0)
					error("funcao chamada ainda nao existe ou eh invalida",line);
				diff = ((int)vecFuncs[vecParms[1]]) - ((int)posAtual) - startCallSize - 5;
				call = concatVecUC(codigo,1,(unsigned char*)&diff,4);
				callSize = 5; /* Código não trata calls com distância entre funções > 31 bits de endereço */
			}
			
			startAndCall = concatVecUC(startCall,startCallSize,call,callSize);
			startAndCallSize = startCallSize + callSize;
			free(startCall);
			free(call);
			
			/* movl -24(%rbp), %edi -> 8b 7d e8 */
			/* movl %eax, N(%rbp) -> 89 45 N */
			{
				unsigned char codigo[] = {0x8b,0x7d,0xe8};
				char stkPos = getStackPos(vecParms[0]);
				unsigned char codigo2[] = {0x89,0x45,stkPos};
				endCall = concatVecUC(codigo,3,codigo2,3);
				endCallSize = 6;
			}
			
			
			*nBytesRet = startAndCallSize + endCallSize;
			vecInstrucoes = concatVecUC(startAndCall,startAndCallSize,endCall,endCallSize);
			free(startAndCall);
			free(endCall);
			break;
		}
		case CMD_OPR: {
			unsigned char* parm1;
			int parm1Size;
			unsigned char* parm2;
			int parm2Size;
			unsigned char* parms;
			int parmsSize;
			/* movl $0x0, %r10d -> 41 ba 00 00 00 00 */
			unsigned char codigoZera[] = {0x41,0xba,0x00,0x00,0x00,0x00};
		
			switch(vecTipoParms[1]) {
				case VPC_VAR: {
					/* addl N(%rbp), %r10d -> 44 03 55 N */
					char stkPos = getStackPos(vecParms[1]);
					unsigned char codigo[] = {0x44,0x03,0x55,stkPos};
					parm1 = concatVecUC(codigoZera,6,codigo,4);
					parm1Size = 10;
					break;
				}
				case VPC_PARM: {
					/* addl %edi, %r10d -> 41 01 fa */
					unsigned char codigo[] = {0x41,0x01,0xfa};
					parm1 = concatVecUC(codigoZera,6,codigo,3);
					parm1Size = 9;
					break;
				}
				case VPC_CONST: {
					/* addl $0xC, %r10d -> 41 81 c2 CC CC CC CC */
					unsigned char codigo[] = {0x41,0x81,0xc2};
					unsigned char* temp = concatVecUC(codigo,3,(unsigned char*)&vecParms[1],4);
					parm1 = concatVecUC(codigoZera,6,temp,7);
					parm1Size = 13;
					free(temp);
					break;
				}
				default:
					printf("Erro (9):	inconsistencia em parametro\n\n");	exit(1);
			}
			
			switch(opr) {
				case OPR_SUM: {
					switch(vecTipoParms[2]) {
						case VPC_VAR: {
							/* addl N(%rbp), %r10d -> 44 03 55 N */
							char stkPos = getStackPos(vecParms[2]);
							unsigned char codigo[] = {0x44,0x03,0x55,stkPos};
							parm2 = alocarTempUC(codigo,4);
							parm2Size = 4;
							break;
						}
						case VPC_PARM: {
							/* addl %edi, %r10d -> 41 01 fa */
							unsigned char codigo[] = {0x41,0x01,0xfa};
							parm2 = alocarTempUC(codigo,3);
							parm2Size = 3;
							break;
						}
						case VPC_CONST: {
							/* addl $0xC, %r10d -> 41 81 c2 CC CC CC CC */
							unsigned char codigo[] = {0x41,0x81,0xc2};
							parm2 = concatVecUC(codigo,3,(unsigned char*)&vecParms[2],4);
							parm2Size = 7;
							break;
						}
						default:
							printf("Erro (11):	inconsistencia em tipo\n\n");	exit(1);
					}
					break;
				}
				case OPR_SUB: {
					switch(vecTipoParms[2]) {
						case VPC_VAR: {
							/* subl N(%rbp), %r10d -> 44 2b 55 N */
							char stkPos = getStackPos(vecParms[2]);
							unsigned char codigo[] = {0x44,0x2b,0x55,stkPos};
							parm2 = alocarTempUC(codigo,4);
							parm2Size = 4;
							break;
						}
						case VPC_PARM: {
							/* subl %edi, %r10d -> 41 29 fa */
							unsigned char codigo[] = {0x41,0x29,0xfa};
							parm2 = alocarTempUC(codigo,3);
							parm2Size = 3;
							break;
						}
						case VPC_CONST: {
							/* subl $0xC, %r10d -> 41 81 ea CC CC CC CC */
							unsigned char codigo[] = {0x41,0x81,0xea};
							parm2 = concatVecUC(codigo,3,(unsigned char*)&vecParms[2],4);
							parm2Size = 7;
							break;
						}
						default:
							printf("Erro (12):	inconsistencia em tipo\n\n");	exit(1);
					}
					break;
				}
				case OPR_MULT: {
					switch(vecTipoParms[2]) {
						case VPC_VAR: {
							/* imull N(%rbp), %r10d -> 44 0f af 55 N */
							char stkPos = getStackPos(vecParms[2]);
							unsigned char codigo[] = {0x44,0x0f,0xaf,0x55,stkPos};
							parm2 = alocarTempUC(codigo,5);
							parm2Size = 5;
							break;
						}
						case VPC_PARM: {
							/* imull %edi,%r10d -> 44 0f af d7 */
							unsigned char codigo[] = {0x44,0x0f,0xaf,0xd7};
							parm2 = alocarTempUC(codigo,4);
							parm2Size = 4;
							break;
						}
						case VPC_CONST: {
							/* imull $0xC, %r10d -> 45 69 d2 CC CC CC CC */
							unsigned char codigo[] = {0x45,0x69,0xd2};
							parm2 = concatVecUC(codigo,3,(unsigned char*)&vecParms[2],4);
							parm2Size = 7;
							break;
						}
						default:
							printf("Erro (13):	inconsistencia em tipo\n\n");	exit(1);
					}
					break;
				}
				default:
					printf("Erro (10):	inconsistencia em operador\n\n");	exit(1);
			}
			
			parms = concatVecUC(parm1,parm1Size,parm2,parm2Size);
			parmsSize = parm1Size + parm2Size;
			free(parm1);
			free(parm2);
			
			/* movl %r10d, N(%rbp) -> 44 89 55 N */
			{
				char stkPos = getStackPos(vecParms[0]);
				unsigned char codigo[] = {0x44,0x89,0x55,stkPos};
				
				*nBytesRet = parmsSize + 4;
				vecInstrucoes = concatVecUC(parms,parmsSize,codigo,4);
				free(parms);
			}
			
			break;
		}
		default:
			printf("Erro (14):	inconsistencia em comando\n\n");	exit(1);
	}
	return vecInstrucoes;
}

/*********************************************************************************************
*		Função:			geracod
*		Descrição:
*			Recebe um arquivo texto com código escrito em SBF e retorna um espaço alocado
*			dinamicamente com código de máquina equivalente aos comandos SBF. Também retorna o
*			endereço, dentro desse espaço, das instruções da última função SBF definida.
*		Parâmetros:
*			arqSBF:	Descritor do arquivo texto já aberto que contém código SBF.
*			codigoRet:	Referência de ponteiro usada para retornar o endereço inicial do código
*									criado.
*			mainSBFRet:	Referência de função que recebe inteiro e retorna inteiro usada para retornar
*									a última função SBF definida.
*********************************************************************************************/
void geracod (FILE *arqSBF, void **codigoRet, funcp *mainSBFRet) {
  Comando cmd;
  int line = 1;
  int  c;
  void* codigo = malloc(TAMANHO_CODIGO);
  unsigned long funcs[MAX_FUNCS];
  int numFuncs = 0;
  unsigned long numInstrucao = 0;
	
	fscanf(arqSBF," ");
  while ((c = fgetc(arqSBF)) != EOF) {
  	int nBytes;
  	unsigned char* vecInstrucoes;
  	int* parmsCmd = NULL;
  	VPC* tipoParmsCmd = NULL;
  	int numParmsCmd = 0;
  	Operador oprCmd = OPR_NONE;
		char varsUsadas[MAX_LOCAL_VARS];
  	
    switch (c) {
      case 'f': { /* function */
        if (fscanf(arqSBF, "unction") != 0)
          error("comando invalido", line);
        cmd = CMD_FUNC;
        break;
      }
      case 'e': { /* end */
        if (fscanf(arqSBF, "nd") != 0)
          error("comando invalido", line);
        cmd = CMD_END;
        break;
      }
      case 'r': {  /* retorno */
        int idx0, idx1;
        char var0, var1, c0, c1;
        VPC typeVar0, typeVar1;
        if (fscanf(arqSBF, "et?%c %c%d%c %c%d", &c0, &var0, &idx0, &c1, &var1, &idx1) != 6)
          error("comando invalido", line);
        if (!isBlank(c0) || !isBlank(c1))
        	error("comando invalido", line);
        if ((typeVar0 = getVPC(var0, idx0)) == VPC_NONE)
        	error("condicao de retorno invalido", line);
        if ((typeVar1 = getVPC(var1, idx1)) == VPC_NONE)
        	error("tipo de retorno invalido", line);
        if (typeVar0 == VPC_VAR && !varsUsadas[idx0])
        	error("variavel usada nao foi definida",line);
        if (typeVar1 == VPC_VAR && !varsUsadas[idx1])
        	error("variavel usada nao foi definida",line);
        cmd = CMD_RET;
        numParmsCmd = 2;
        tipoParmsCmd = (VPC*) malloc(numParmsCmd*sizeof(VPC));
        parmsCmd = (int*) malloc(numParmsCmd*sizeof(int));
        if (!tipoParmsCmd || !parmsCmd) {printf("Erro(2): falta de memoria\n\n");	exit(1);}
        tipoParmsCmd[0] = typeVar0;
        tipoParmsCmd[1] = typeVar1;
        parmsCmd[0] = idx0;
        parmsCmd[1] = idx1;
        break;
      }
      case 'v': {  /* atribuicao */
        int idx0;
        char var0 = c, c0, c1, expr;
        if (fscanf(arqSBF, "%d%c =%c %c",&idx0, &c0, &c1, &expr) != 4)
          error("comando invalido",line);
        if (!isBlank(c0) || !isBlank(c1))
        	error("comando invalido",line);
        if (getVPC(var0,idx0) == VPC_NONE)
        	error("indice de variavel invalido", line);
        if (expr == 'c') { /* call */
          int f, idx1;
          char var1, c3, c4;
          VPC typeVar1;
          if (fscanf(arqSBF, "all%c %d%c %c%d", &c3, &f, &c4, &var1, &idx1) != 5)
            error("comando invalido",line);
          if (!isBlank(c3) || !isBlank(c4))
          	error("comando invalido",line);
          if ((typeVar1 = getVPC(var1,idx1)) == VPC_NONE)
          	error("parametro de call invalido",line);
        	if (typeVar1 == VPC_VAR && !varsUsadas[idx1])
        		error("variavel usada nao foi definida",line);
        	varsUsadas[idx0] = 1;
          cmd = CMD_CALL;
          numParmsCmd = 3;
		      tipoParmsCmd = (VPC*) malloc(numParmsCmd*sizeof(VPC));
		      parmsCmd = (int*) malloc(numParmsCmd*sizeof(int));
		      if (!tipoParmsCmd || !parmsCmd) {printf("Erro(3): falta de memoria\n\n");	exit(1);}
		      tipoParmsCmd[0] = VPC_VAR;
		      tipoParmsCmd[1] = VPC_NONE;
		      tipoParmsCmd[2] = typeVar1;
		      parmsCmd[0] = idx0;
		      parmsCmd[1] = f;
		      parmsCmd[2] = idx1;
        } else { /* operação aritmética */
          int idx1, idx2;
          char var1 = expr, var2, op, c3, c4;
          VPC typeVar1, typeVar2;
          Operador typeOp;
          if (fscanf(arqSBF, "%d%c %c%c %c%d", &idx1, &c3, &op, &c4, &var2, &idx2) != 6)
            error("comando invalido", line);
          if (!isBlank(c3) || !isBlank(c4))
          	error("comando invalido",line);
          if ((typeOp = getOpr(op)) == OPR_NONE)
          	error("operador invalido",line);
          if ((typeVar1 = getVPC(var1,idx1)) == VPC_NONE)
          	error("lado esquerdo da operacao invalido",line);
          if ((typeVar2 = getVPC(var2,idx2)) == VPC_NONE)
          	error("lado direito da operacao invalido",line);
          if (typeVar1 == VPC_VAR && !varsUsadas[idx1])
        		error("variavel usada nao foi definida",line);
        	if (typeVar2 == VPC_VAR && !varsUsadas[idx2])
        		error("variavel usada nao foi definida",line);
        	varsUsadas[idx0] = 1;
          cmd = CMD_OPR;
          numParmsCmd = 3;
		      tipoParmsCmd = (VPC*) malloc(numParmsCmd*sizeof(VPC));
		      parmsCmd = (int*) malloc(numParmsCmd*sizeof(int));
		      if (!tipoParmsCmd || !parmsCmd) {printf("Erro(4): falta de memoria\n\n");	exit(1);}
		      tipoParmsCmd[0] = VPC_VAR;
		      tipoParmsCmd[1] = typeVar1;
		      tipoParmsCmd[2] = typeVar2;
		      parmsCmd[0] = idx0;
		      parmsCmd[1] = idx1;
		      parmsCmd[2] = idx2;
		      oprCmd = typeOp;
        }
        break;
      }
      default: error("comando desconhecido", line);
    }
    if (!endCmdLine(arqSBF)) /* Todo comando termina com um espaço ou quebra de linha ou EOF.*/
    	error("comando invalido", line);
        
		line ++;
		
		vecInstrucoes = obterInstrucoes	(cmd, tipoParmsCmd, parmsCmd, numParmsCmd,
																		oprCmd, funcs, numFuncs, numInstrucao, line, &nBytes);
		if (vecInstrucoes) {
			if (cmd == CMD_FUNC) {
				int i;
				funcs[numFuncs] = numInstrucao;
				numFuncs++;
				for (i = 0; i < MAX_LOCAL_VARS; i++) {
					varsUsadas[i] = 0;
				}
			}
			colocarCodigo(codigo,numInstrucao,vecInstrucoes,nBytes);
			numInstrucao += (unsigned long) ((unsigned int) nBytes);
			free(vecInstrucoes);
		} else
			error("problema grave desconhecido",line);
  }
  if (numFuncs > 0) {
		*codigoRet = codigo;
		*mainSBFRet = (funcp) (((unsigned char*)codigo) + funcs[numFuncs-1]);
  } else {
  	free(codigo);
  	*codigoRet = NULL;
  	*mainSBFRet = NULL;
  }
}

/*********************************************************************************************
*		Função:			liberacod
*		Descrição:
*			Recebe um ponteiro para um código de máquina criado pela "geracod" e libera o espaço
*			alocado.
*		Parâmetros:
*			codigo:	Ponteiro para código a ser liberado.	
*********************************************************************************************/
void liberacod(void* codigo) {
	free(codigo);
}
