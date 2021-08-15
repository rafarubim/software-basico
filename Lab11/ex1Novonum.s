/*int novonum(void) {
  int minhalocal;
  printf("numero: ");
  scanf("%d",&minhalocal);
  return minhalocal;
}*/
.data
Str1:	.string "numero: "
Str2:	.string	"%d"

.text
.globl novonum
novonum:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	/* minhalocal = -8(%rbp) */
	
	/* printf */
	movq $Str1, %rdi
	movl $0, %eax
	call printf
	/* end printf */
	
	/* scanf */
	movq $Str2, %rdi
	leaq -8(%rbp), %rsi
	call scanf
	/* end scanf */
	
	movl -8(%rbp), %eax /* return minhalocal */
	
	leave
	ret
