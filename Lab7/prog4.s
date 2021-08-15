/*#include <stdio.h>

int nums[4] = {65, -105, 111, 34};

int main (void) {
  int i;
  int s = 0;

  for (i=0;i<4;i++)
    s = s+nums[i];

  printf ("soma = %d\n", s);

  return 0;
}*/

.data
nums: .int 65, -105, 111, 34
Sf: .string "soma = %d\n"

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

  movq $nums, %r12

  movl $0, %ebx
  
  movl $0, %ecx

L1:

  cmpl $4, %ecx
  jge L2

  movslq %ecx, %r8
  imulq $4, %r8
  movq %r12, %r9
  addq %r8, %r9
 
  addl (%r9), %ebx

  incl %ecx
  jmp L1
L2:

  movl %ebx, %eax

/* PRINTF*/
  movq $Sf, %rdi
  movl %eax, %esi
  movl $0, %eax
  call printf
/* ENDPRINTF*/

/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq    -16(%rbp), %r12 /* recupera r12 */
  movq    -8(%rbp), %rbx  /* recupera rbx */
  leave
  ret      
/***************************************************************/


