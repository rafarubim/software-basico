.text
.globl myotherwrite

myotherwrite:
	pushq %rbp
	movq %rsp, %rbp
	movl %rdi, %r10d
	/* fd é %r10d */
	movq %rsi, %r11
	/* s é %r11 */
	movq %r11, %rdi
	call strlen
	movl %r10d, %rdi
	movq %r11, %rsi
	movl %eax, %edx
	movq $1, %rax
	syscall
	leave
	ret
