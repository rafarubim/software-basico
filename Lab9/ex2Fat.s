/*int fat (int n) {
  if (n==0) return 1;
  else return n*fat(n-1);
}*/

.text
.globl fat

fat:

	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %rbx, -8(%rbp)
	
	cmpl $0, %edi
	jne else
	
	movl $1, %eax	
	
	jmp end
else:
	
	movl %edi, %ebx
	
	decl %edi
	call fat
	
	imull %ebx, %eax
	
end:

	movq -8(%rbp), %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
	
