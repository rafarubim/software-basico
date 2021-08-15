/*float foo (double a, float b) {
  return (a+b)*(a-b);
}*/
.text
.globl foo

foo:
	pushq %rbp
	movq %rsp, %rbp
	movsd %xmm0, %xmm2
	cvtss2sd %xmm1, %xmm1
	addsd %xmm1, %xmm2
	movsd %xmm0, %xmm3
	subsd %xmm1, %xmm3
	mulsd %xmm2, %xmm3
	cvtsd2ss %xmm3, %xmm0
	leave
	ret
