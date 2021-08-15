/*void foo (int a[], double b[], int n) {
  int *pint;
  double *pdouble;

  for (pint = a, pdouble = b; n-- ; pint++, pdouble++) {
    *pdouble = (double)*pint * 2.0;
  }
}*/
.text
.globl foo
foo:
	pushq %rbp
	movq %rsp, %rbp
	/* parâmetros são respectivos registradores */
	/* %ecx é variável temporária */
	/* pint é %r10 */
	/* pdouble é %r11 */
	
	movq %rdi, %r10
	movq %rsi, %r11
for1:
	cmpl $0, %edx
	je endFor1
	decl %edx
	
	cvtsi2sd (%r10), %xmm0
	movl $2, %ecx
	cvtsi2sd %ecx, %xmm1
	mulsd %xmm1, %xmm0
	movsd %xmm0, (%r11)
	
	addq $4, %r10
	addq $8, %r11
	jmp for1
endFor1:

	leave
	ret
