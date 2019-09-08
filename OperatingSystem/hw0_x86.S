.section .data
output1:
	.asciz "sum = "

number:
	.int 0
enter:
	.asciz "\n"
.section .text
	.globl _start
_start:
	mov	$1, %eax
	mov	$number, %ecx
	mov	$10, %ebx
L1:
	add	%eax, (%ecx)
	inc	%eax
	cmp	%eax, %ebx
	jge	L1

	#mov	%ecx, number
	movl	$4, %eax
	movl	$1, %ebx
	movl	$number, %ecx
	movl	$4, %edx
	int 	$0x80

	movl	$1, %eax
	movl	$0, %ebx
	int	$0x80

