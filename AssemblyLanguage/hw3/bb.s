	.file	"bb.c"
	.text
	.p2align 4,,15
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB0:
	.cfi_startproc
	subl	$1, %esi
	testl	%esi, %esi
	jle	.L1
	leaq	1(%rdi), %r9
	.p2align 4,,10
	.p2align 3
.L3:
	leal	-1(%rsi), %r8d
	movq	%rdi, %rax
	movq	%r8, %rsi
	addq	%r9, %r8
	.p2align 4,,10
	.p2align 3
.L5:
	movzbl	(%rax), %edx
	movzbl	1(%rax), %ecx
	cmpb	%cl, %dl
	jle	.L4
	movb	%cl, (%rax)
	movb	%dl, 1(%rax)
.L4:
	addq	$1, %rax
	cmpq	%rax, %r8
	jne	.L5
	testl	%esi, %esi
	jne	.L3
.L1:
	rep ret
	.cfi_endproc
.LFE0:
	.size	bubble_sort, .-bubble_sort
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
