	.file	"matrix.c"
	.text
	.p2align 4,,15
	.globl	matrix
	.type	matrix, @function
matrix:
.LFB52:
	.cfi_startproc
	movq	%rcx, %rax
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	sarq	$5, %rax
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	testq	%rax, %rax
	movq	%rdi, -16(%rsp)
	movq	%rdx, -8(%rsp)
	jle	.L1
	movq	%rcx, %r12
	movq	%rcx, %rbx
	addq	$1, %rax
	leaq	-1(%r12), %r13
	salq	$8, %rbx
	salq	$5, %rax
	leaq	32(%rdx), %r15
	leaq	0(,%rcx,8), %r11
	movq	%rbx, -24(%rsp)
	movq	%rsi, %r14
	movq	%rax, -32(%rsp)
	movl	$32, %ecx
	andq	$-4, %r13
.L3:
	leaq	-32(%rcx), %rax
	movq	-16(%rsp), %r10
	movq	-8(%rsp), %r9
	xorl	%ebp, %ebp
	xorl	%ebx, %ebx
	movq	%rax, -40(%rsp)
	.p2align 4,,10
	.p2align 3
.L6:
	leaq	0(%r13,%rbp), %rax
	movq	-40(%rsp), %rdi
	movq	%r14, %r8
	leaq	(%r15,%rax,8), %rsi
	.p2align 4,,10
	.p2align 3
.L5:
	movsd	(%r10,%rdi,8), %xmm0
	movq	%r8, %rdx
	movq	%r9, %rax
	.p2align 4,,10
	.p2align 3
.L4:
	movsd	(%rdx), %xmm1
	addq	$32, %rax
	addq	$32, %rdx
	mulsd	%xmm0, %xmm1
	addsd	-32(%rax), %xmm1
	movsd	%xmm1, -32(%rax)
	movsd	-24(%rdx), %xmm1
	mulsd	%xmm0, %xmm1
	addsd	-24(%rax), %xmm1
	movsd	%xmm1, -24(%rax)
	movsd	-16(%rdx), %xmm1
	mulsd	%xmm0, %xmm1
	addsd	-16(%rax), %xmm1
	movsd	%xmm1, -16(%rax)
	movsd	-8(%rdx), %xmm1
	mulsd	%xmm0, %xmm1
	addsd	-8(%rax), %xmm1
	movsd	%xmm1, -8(%rax)
	cmpq	%rsi, %rax
	jne	.L4
	addq	$1, %rdi
	addq	%r11, %r8
	cmpq	%rcx, %rdi
	jl	.L5
	addq	$1, %rbx
	addq	%r11, %r9
	addq	%r11, %r10
	addq	%r12, %rbp
	cmpq	%rbx, %r12
	jne	.L6
	addq	$32, %rcx
	addq	-24(%rsp), %r14
	cmpq	%rcx, -32(%rsp)
	jne	.L3
.L1:
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
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
.L13:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L14:
	movsd	%xmm1, (%rcx,%rax,8)
	movq	$0x000000000, (%rsi,%rax,8)
	movsd	%xmm0, (%rdi,%rax,8)
	addq	$1, %rax
	cmpq	$1024, %rax
	jne	.L14
	addq	$8192, %rcx
	addq	$8192, %rdi
	addq	$8192, %rsi
	cmpq	%r8, %rcx
	jne	.L13
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
	jne	.L19
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
.L19:
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
