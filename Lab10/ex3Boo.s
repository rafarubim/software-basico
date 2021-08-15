/*struct X {
  int val1;
  int val2;
};

void boo (struct X *px, int n, int val) {
  while (n--) {
    px->val2 = f(px->val1, val);
    px++;
  }
}*/

.text
.globl boo

boo:
	
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %r10, -8(%rbp)
	movl %r9d, -12(%rbp)
	movl %r11d, -16(%rbp)
	
	/*px é %r10, n é %r9d, val é normal*/
	movq %rdi, %r10
	movl %esi, %r9d
	
while:
	/*%r11d é tqmporário para o teste do while*/
	movl %r9d, %r11d
	decl %r9d
	cmpl $0, %r11d
	je endWhile
	
	/*f - 2args*/
	movl (%r10), %edi
	movl %edx, %esi
	call f
	/*f - end*/
	
	movl %eax, 4(%r10)
	add $8, %r10
		
	jmp while
endWhile:
	
	movl -16(%rbp), %r11d
	movl -12(%rbp), %r9d
	movq -8(%rbp), %r10
	movq %rbp, %rsp
	popq %rbp
	ret
