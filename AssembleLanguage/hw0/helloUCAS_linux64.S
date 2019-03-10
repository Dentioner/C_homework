#hello.S
.section .data
	output:	.ascii  "University of CAS\n"
.section .text
.globl _start
_start:
/* output  like printf */
	movq	$1, %rax
	movq	$1, %rdi
	movq	$output, %rsi
	movq	$19, %rdx
	syscall
/* exit */
	movq	$60, %rax
	movq	$0, %rdi
	syscall
