/*void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}*/

.text
.globl foo

foo:

	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %rbx, -8(%rbp)
	
	/*%ecx é i*/
	movl $0, %edx /* edx é s*/

	movl $0, %ecx
	
forLoop:

	cmpl %esi, %ecx
	jge endFor
	
	/*%rbx é 'i' estendido*/
	movslq %ecx, %rbx
	imulq $4, %rbx
	/*%rbx é endereço indexado*/
	addq %rdi, %rbx
	
	addl (%rbx), %edx
	
	cmpl $0, (%rbx)
	jne notFor
	
	movl %edx, (%rbx)
	movl $0, %edx
	
notFor:
	
	incl %ecx
	jmp forLoop

endFor:
	
	movq -8(%rbp), %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
