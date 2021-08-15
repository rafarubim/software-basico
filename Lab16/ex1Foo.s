/*int foo (int x) {
  return x+1;
}*/
.text
.globl foo

foo:
	pushq %rbp
	movq %rsp, %rbp
	movl %edi, %eax
	addl $1, %eax
	leave
	ret
