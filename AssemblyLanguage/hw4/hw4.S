.code32
.section .data
	inputdata:
		.byte 0,0,0,0,0,0,0,0,0,0
	
	enter:
		.asciz "\n"
	signsymbol:
		.asciz "-"
	justzero:
		.asciz "0"

.section .text
.globl _start
_start:
	

	mov $3, %eax
	mov $1, %ebx
	mov $inputdata, %ecx
	mov $10, %edx
	int	$0x80
	
	movl	8(%ecx), %ebx
	pushl %ebx
	movl	4(%ecx), %ebx
	pushl %ebx
	movl	(%ecx), %ebx
	pushl %ebx
	
	call print_hex
	mov	$1,%eax
	mov	$0,%ebx
	int	$0x80

.type print_hex, @function
print_hex:
	push	%ebp
	mov	%esp, %ebp
	sub	$24, %esp
	mov	%ebx, -4(%ebp)
	mov	%edi, -8(%ebp)
	mov	%esi, -12(%ebp)
	
	mov	$0, %edi
	mov	10(%ebp), %ecx 
	mov  14(%ebp), %ebx  
	
	movb	$'0', 0(%esp)
	movb	$'x', 1(%esp)
	movb	$0, 10(%esp)																																																																																							

	
	mov		%ecx, %edx 
	
	
	cmpb	$0x38, %dl	
	jl		normal
	mov		$1, %edi		

	mov		%ebp, %eax
	add		$10, %eax 
	mov		$8, %ecx 
	mov		$0, %esi
	
loooop:
	movb	(%eax), %bl
	cmpb	$0x39, %bl
	jna		L1
	jmp		L2
L1: 	
	subb		$0x30, %bl
	jmp		L3
L2: 
	subb		$87, %bl
	jmp		L3
L3: 
	xorb		$0xf, %bl
	cmp		$7,	%esi
	jne		L3_5
	addb		$1, %bl 
L3_5:
	cmpb	$9, %bl
	jna		L4
	jmp		L5
	
L4: 
	addb	$0x30, %bl
	jmp		L6
L5: 
	addb	$87, %bl
	jmp		L6
L6:
	movb	%bl, (%eax)
	inc		%eax
	inc		%esi

loop loooop

	mov	10(%ebp), %ecx 
	mov  14(%ebp), %ebx  




	
normal:
	movl		%ecx, 2(%esp)
	movl		%ebx,  6(%esp)
	
	cmp		$1, %edi
	jne		print0x
	


printsign:
	movl	$4, %eax
	movl	$1, %ebx
	movl	$signsymbol, %ecx
	movl	$1, %edx
	int	$0x80


print0x:
	movl	$4, %eax
	movl	$1, %ebx
	movl	%esp, %ecx
	movl	$2, %edx
	int	$0x80

	addl	$2, %esp
	movl	$0, %esi
	
	
printmain:	
	mov	$4, %eax
	mov	$1, %ebx

	subl	$1, %esp	
findno0:
	addl	$1, %esi
	addl	$1, %esp
	cmpb	$0x30, (%esp)
	je	findno0
printelement:	
	
	movl	$4, %eax
	movl	$1, %ebx
	mov	%esp, %ecx
	mov	$1, %edx
	int	$0x80
	addl	$1, %esp
	cmpb	$0, (%esp)
	jne		printelement
	
	cmp	$9, %esi
	jne		printenter
	movl	$4, %eax
	movl	$1, %ebx
	mov	$justzero, %ecx
	mov	$1, %edx
	int	$0x80

printenter:
	movl	$4, %eax
	movl	$1, %ebx
	movl	$enter, %ecx
	movl	$1, %edx
	int	$0x80

	mov	-4(%ebp), %ebx
	mov	-8(%ebp), %edi
	mov	-12(%ebp), %esi
	leave
	ret
