.text
.globl myopen
.globl myread
.globl mywrite
.globl myclose

myopen:
	pushq %rbp
	movq %rsp, %rbp
	movl %edx, tamBufferAberto
	movq $2, %rax
	syscall
	leave
	ret

myread:
	pushq %rbp
	movq %rsp, %rbp
	movl tamBufferAberto, %edx
	movq $0, %rax
	syscall
	leave
	ret
	
mywrite:
	pushq %rbp
	movq %rsp, %rbp
	movq $1, %rax
	syscall
	leave
	ret
	
myclose:
	pushq %rbp
	movq %rsp, %rbp
	movq $3, %rax
	syscall
	leave
	ret
