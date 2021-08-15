/*struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  for (; x != NULL; x = x->next)
    a += x->val;
  return a;
}*/

.text
.globl add

add:

	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movl %r10d, -8(%rbp)
	
	/*x é %rdi*/
	
	/*a é %r10d*/
	movl $0, %r10d
	
addFor:
	cmpq $0, %rdi
	je endFor
	
	addl (%rdi), %r10d
	
	movq 8(%rdi), %rdi
	jmp addFor
endFor:
	
	/*return a*/
	movl %r10d, %eax
	
	movl -8(%rbp), %r10d

	movq %rbp, %rsp
	popq %rbp
	ret
