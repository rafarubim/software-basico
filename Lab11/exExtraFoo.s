/*int foo (int a[], int tam) {
  int i;
  int b[tam];
  for (i = 0; i<tam; i++)
    b[i] = calcula (&(a[i]), tam-i);
  return calcula (b, tam);
}*/
.text
.globl foo
foo:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq %r12, -32(%rbp) /* a = %r12 */
	movl %r13d, -24(%rbp) /* tam = %r13d */
	movl %r14d, -20(%rbp) /* i = %r14d */
	movl %r15d, -16(%rbp) /* tam (inalterado) */
	movq %rbx, -8(%rbp) /* variável temporária */
	movl $0, %ebx
	movl $0, %r13d
for1:
	cmpl %r15d, %r13d
	jge endfor1
	
	incl %ebx
	addl $4, %r15d
	subq $16, %rsp
	
	jmp for1
endfor1:
	movl %r15d, %r13d
	imull $16, %ebx
	addl $32, %ebx
	/* b = (%rbp) - %ebx */
	
	movl $0, %r14d
for2:
	cmpl $4, %r14d
	jge endfor2
	
	/* calcula */
	movq $4, %rdi
	imulq %r14d, %rdi
	addq %r12, %rdi
	movl %r13d, %esi
	subl %r14d, %esi
	call calcula
	/* end calcula */
	
	
	
	incl %r14d
	jmp for2
endfor2:
	
