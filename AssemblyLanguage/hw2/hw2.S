.section .data
iostring:
  .asciz "ab1g2hA0H56po9wK78nB"
enter:
  .asciz "\n"
.section .text
.globl _start
_start:
	mov $iostring, %ebx
	mov %ebx, %edx
L1:
	mov (%ebx), %al
	test %al, %al
        je L2
        cmp $'a', %al
        jb L3
        cmp $'z', %al
        ja L3
        sub $32, %al	
        
L3:
        mov %al, (%edx)
	inc %edx
	inc %ebx
        jmp L1
 
L2:
        mov %al, (%edx)
	sub $20, %edx	

#output
	movl	$4, %eax
	movl	$1, %ebx
	movl	%edx, %ecx
	movl	$20, %edx
	int	$0x80

	movl	$4, %eax
	movl	$1, %ebx
	movl	$enter, %ecx
	movl	$1, %edx
	int	$0x80
#exit
	movl	$1, %eax
	movl	$0, %ebx
	int	$0x80
