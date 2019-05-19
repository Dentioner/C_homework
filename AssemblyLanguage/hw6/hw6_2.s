	.file	"hw6_2.c"
	.text
	.p2align 4,,15
	.globl	shld5
	.type	shld5, @function
shld5:
.LFB23:
	.cfi_startproc
#APP
# 9 "hw6_2.c" 1
	SHL $5, %edi
	SHR $27, %esi
	OR %edi, %esi
	MOV %esi, %eax
# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE23:
	.size	shld5, .-shld5
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"a=? b=?"
.LC1:
	.string	"%u %u"
.LC2:
	.string	"%u\n"
.LC3:
	.string	"real result = %u\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	leaq	.LC0(%rip), %rdi
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	puts@PLT
	leaq	4(%rsp), %rdx
	leaq	.LC1(%rip), %rdi
	movq	%rsp, %rsi
	xorl	%eax, %eax
	call	__isoc99_scanf@PLT
	leaq	.LC2(%rip), %rsi
	movl	4(%rsp), %eax
	movl	(%rsp), %edx
	movl	$1, %edi
#APP
# 9 "hw6_2.c" 1
	SHL $5, %edx
	SHR $27, %eax
	OR %edx, %eax
	MOV %eax, %edx
# 0 "" 2
#NO_APP
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movl	(%rsp), %edx
	movl	4(%rsp), %eax
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	shrl	$27, %eax
	sall	$5, %edx
	orl	%eax, %edx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L6
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
