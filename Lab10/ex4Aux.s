/*int f (int x) { return x+2; }

void map2 (int* um, int * outro, int n) {
  int i;
  for (i=0; i<n; i++)
    *(outro+i) = f(*(um+i));
}*/

.text
.globl f
.globl map2

f:
	
	pushq %rbp
	movq %rsp, %rbp
	
	movl %edi, %eax
	addl $2, %eax
	
	movq %rbp, %rsp
	pop %rbp
	ret


map2:
	
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq %r12, -8(%rbp)
	movq %r13, -16(%rbp)
	movl %r14d, -20(%rbp)
	movl %r15d, -24(%rbp)
	movq %r10, -32(%rbp)
	
	/*Args: %r12, %r13, %r14d*/
	movq %rdi, %r12
	movq %rsi, %r13
	movl %edx, %r14d
	
	/*i é %r15d*/
	
	movl $0, %r15d
for:
	cmp %edx, %r15d
	jge endFor
	
	/*f- 1 arg*/
	/*%r10 é endereço temporário (um+i)*/
	movslq %r15d, %r10
	imulq $4, %r10
	addq %r12, %r10
	
	movl (%r10), %edi
	call f 
	/*f- end*/
	/*%r10 é endereço temporário (outro+i)*/
	movslq %r15d, %r10
	imulq $4, %r10
	addq %r13, %r10
	
	/* *(outro+i) = retorno de f*/
	movl %eax, (%r10)
	
	incl %r15d
	jmp for
endFor:
	
	movq -8(%rbp), %r12
	movq -16(%rbp), %r13
	movl -20(%rbp), %r14d
	movl -24(%rbp), %r15d
	movq -32(%rbp), %r10
	movq %rbp, %rsp
	popq %rbp
	ret
