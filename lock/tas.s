.global tas 
.type tas, @function

tas:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1, %edx
	xchgb	(%rax), %dl
	movzbl	%dl, %eax
	popq	%rbp
	ret
