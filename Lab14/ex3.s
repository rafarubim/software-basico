/*double foo2 (float a, float b) {
  return sin(a) + cos(b);
}*/
.text
.globl foo2
foo2:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movss %xmm0, -4(%rbp)
	cvtss2sd %xmm1, %xmm0
	call cos
	movsd %xmm0, %xmm1
	cvtss2sd -4(%rbp), %xmm0
	movsd %xmm1, -8(%rbp)
	call sin
	movsd -8(%rbp), %xmm1
	addsd %xmm1, %xmm0
	leave
	ret
