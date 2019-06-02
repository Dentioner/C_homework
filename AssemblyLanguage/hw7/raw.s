	.file	"raw.c"
	.text
	.p2align 4,,15
	.globl	matrix
	.type	matrix, @function
matrix:
.LFB52:
	.cfi_startproc
	testq	%rcx, %rcx
	jle	.L9
	movq	%rcx, %rax
	leaq	0(,%rcx,8), %r8
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	salq	$10, %rax
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdx, %rbx
	addq	%rax, %rcx
	subq	%r8, %rbx
	leaq	(%rdi,%r8), %r9
	leaq	(%rdi,%rcx,8), %rbp
	subq	%rdi, %rbx
	subq	%rdi, %rdx
	.p2align 4,,10
	.p2align 3
.L3:
	leaq	(%r9,%rbx), %r8
	leaq	(%rdx,%r9), %r11
	movq	%rsi, %r10
	.p2align 4,,10
	.p2align 3
.L6:
	movsd	(%r8), %xmm1
	movq	%r10, %rcx
	movq	%rdi, %rax
	.p2align 4,,10
	.p2align 3
.L4:
	movsd	(%rax), %xmm0
	addq	$8, %rax
	addq	$8192, %rcx
	mulsd	-8192(%rcx), %xmm0
	cmpq	%rax, %r9
	addsd	%xmm0, %xmm1
	movsd	%xmm1, (%r8)
	jne	.L4
	addq	$8, %r8
	addq	$8, %r10
	cmpq	%r11, %r8
	jne	.L6
	addq	$8192, %r9
	addq	$8192, %rdi
	cmpq	%r9, %rbp
	jne	.L3
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore 3
	.cfi_restore 6
	rep ret
	.cfi_endproc
.LFE52:
	.size	matrix, .-matrix
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"time elapse %ld s: %ld us\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB53:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	$8388608, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$48, %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
	call	malloc@PLT
	movl	$8388608, %edi
	movq	%rax, %rbx
	call	malloc@PLT
	movl	$8388608, %edi
	movq	%rax, %rbp
	call	malloc@PLT
	leaq	8388608(%rbx), %r8
	movsd	.LC0(%rip), %xmm1
	movsd	.LC1(%rip), %xmm0
	movq	%rax, %r12
	movq	%rbx, %rcx
	movq	%rbp, %rdi
	movq	%rax, %rsi
	.p2align 4,,10
	.p2align 3
.L14:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L15:
	movsd	%xmm1, (%rcx,%rax,8)
	movq	$0x000000000, (%rsi,%rax,8)
	movsd	%xmm0, (%rdi,%rax,8)
	addq	$1, %rax
	cmpq	$1024, %rax
	jne	.L15
	addq	$8192, %rcx
	addq	$8192, %rdi
	addq	$8192, %rsi
	cmpq	%r8, %rcx
	jne	.L14
	xorl	%esi, %esi
	movq	%rsp, %rdi
	call	gettimeofday@PLT
	movl	$1024, %ecx
	movq	%r12, %rdx
	movq	%rbx, %rdi
	movq	%rbp, %rsi
	call	matrix
	leaq	16(%rsp), %rdi
	xorl	%esi, %esi
	call	gettimeofday@PLT
	movl	24(%rsp), %ecx
	movl	16(%rsp), %edx
	leaq	.LC3(%rip), %rsi
	subq	8(%rsp), %rcx
	subq	(%rsp), %rdx
	xorl	%eax, %eax
	movl	$1, %edi
	call	__printf_chk@PLT
	xorl	%eax, %eax
	movq	40(%rsp), %rbx
	xorq	%fs:40, %rbx
	jne	.L20
	addq	$48, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L20:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE53:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1073741824
	.align 8
.LC1:
	.long	0
	.long	-1073741824
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
