.section .data
stringvar:
	.ascii "0123456789abcdef"
.section .text
.globl _start
_start:
			
			mov		$8, %ecx
			movl		$stringvar, %ebx
	L1:
			movw	(%ebx), %ax
			xchg		%al, %ah
			movw	%ax, (%ebx)
			add		$2, %ebx
			Loop L1

#output
			movl		$4, %eax
			movl		$1, %ebx
			movl		$stringvar, %ecx
			movl		$16, %edx
			int		$0x80
#exit
			movl		$1, %eax
			movl		$0, %ebx
			int		$0x80
