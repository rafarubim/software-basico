/*double foo3 (double *a, int n) {
  int i;
  double r = 0.0;
  for (i=0; i<n; i++) {
    r += sin(*a);
    a++;
  }
  return r;
}*/
.text
.globl foo3
foo3:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq %r12, -8(%rbp) /* calee save */
	movl %r13d, -12(%rbp) /* calee save */
	movl %r14d, -16(%rbp) /* calee save */
	movl %r15d, -28(%rbp) /* calee save */ /* Variável temporária */
	movq %rdi, %r12 /* a é r12 */
	movl %esi, %r13d /* n é %r13d */
	/* i é %r14d */
	/* r é -24(%rbp) */
	movl $0, %r15d
	cvtsi2sd %r15d, %xmm0
	movsd %xmm0, -24(%rbp)
	
	movl $0, %r14d
for1:
	cmpl %r13d, %r14d
	jge endFor1
	
	movsd (%r12), %xmm0
	call sin
	addsd -24(%rbp), %xmm0
	movsd %xmm0, -24(%rbp)
	
	addq $8, %r12
	
	incl %r14d
	jmp for1
endFor1:
	
	movsd -24(%rbp), %xmm0 /* return r */
	
	movq -8(%rbp), %r12 /* calee unsave */
	movl -12(%rbp), %r13d /* calee unsave */
	movl -16(%rbp), %r14d /* calee unsave */
	movl -28(%rbp), %r15d /* calee unsave */
	leave
	ret
