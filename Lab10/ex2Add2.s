/*int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}*/

.text
.globl add2

add2:
	
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %r10, -8(%rbp)
	
	/*x é r10*/
	movq %rdi, %r10
	
	cmpq $0, %r10
	jne else
	
	/*return 0*/
	movl $0, %eax
	
	jmp endIf
else:
	
	/*add2(x->next)*/
	movq 8(%r10), %rdi
	call add2
	/*retorno de add2 é %eax*/
	/*return x->value + ...*/
	addl (%r10), %eax
	
endIf:
	
	movq -8(%rbp), %r10
	movq %rbp, %rsp
	popq %rbp
	ret
