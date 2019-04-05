	.file	"test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	leaq	.LC0(%rip), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$72, %rsp
	.cfi_def_cfa_offset 112
	movq	%rsp, %rbp
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	leaq	40(%rbp), %r13
	movq	%rbp, %rbx
	.p2align 4,,10
	.p2align 3
.L2:
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movq	%r12, %rdi
	addq	$4, %rbx
	call	__isoc99_scanf@PLT
	cmpq	%r13, %rbx
	jne	.L2
	leaq	.LC0(%rip), %r12
	movl	$0, 44(%rsp)
	.p2align 4,,10
	.p2align 3
.L3:
	movl	0(%rbp), %edx
	xorl	%eax, %eax
	movq	%r12, %rsi
	movl	$1, %edi
	addq	$4, %rbp
	call	__printf_chk@PLT
	cmpq	%rbx, %rbp
	jne	.L3
	xorl	%eax, %eax
	movq	56(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L9
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
