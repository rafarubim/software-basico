/*#include <stdio.h>

int main() {

  for (int i = 1; i <= 10; i++)
    printf("%d\n", i*i);

  return 0;
}*/

.data
Sf: .string "%d\n"

.text
.globl main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)  /* guarda rbx */
  movq    %r12, -16(%rbp)  /* guarda r12 */
/********************************************************/

  movl $1, %ebx
  
L1:

  cmpl $10, %ebx
  jg L2

  movl %ebx, %eax

  imull %eax, %eax

  /* PRINTF */
  movq $Sf, %rdi
  movl %eax, %esi
  movl $0, %eax
  call printf
  /* END PRINTF*/

  incl %ebx
  jmp L1

L2:

/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq    -16(%rbp), %r12 /* recupera r12 */
  movq    -8(%rbp), %rbx  /* recupera rbx */
  leave
  ret      
/***************************************************************/










