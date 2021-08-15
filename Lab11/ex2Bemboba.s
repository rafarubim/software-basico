/*int bemboba (int num) {
  int local[4];
  int *a;
  int i;

  for (i=0,a=local;i<4;i++) {
    *a = num;
    a++;
  }
  return addl (local, 4);
}*/
.text
.globl bemboba
bemboba:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movl %r12d, -32(%rbp) /* num = %r12d */
	movl %r13d, -28(%rbp) /* i = %r13d */
	movq %r14, -24(%rbp) /* a = %r14 */
	/* local = (%rbp) - 16 */
	
	movl %edi, %r12d
	
	/* for */
	movl $0, %r13d
	leaq -16(%rbp), %r14
for1:
	cmpl $4, %r13d
	jge endfor1
	
	movl %r12d, (%r14)
	addq $4, %r14
	
	incl %r13d
	jmp for1
endfor1:
	/* end for */
	
	/* addl */
	leaq -16(%rbp), %rdi
	movl $4, %esi
	call addl
	/* end addl */
	
	/* return addl(..) */
	
	movl -32(%rbp), %r12d
	movl -28(%rbp), %r13d
	movq -24(%rbp), %r14
	leave
	ret
