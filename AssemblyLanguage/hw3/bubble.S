.code32
.section .data
iostring:
  .asciz "ab1g2hA0H56po9wK78nB"
.section .text
.globl _start
_start:
	subl		$8, %esp
	movl	$20, 4(%esp)     
   	movl	$iostring, (%esp)

	call	bubble_sort
#output

	call	as_puts
#exit
	call	as_exit




        .globl  bubble_sort
        .type   bubble_sort, @function
bubble_sort:
.LFB0:
        pushl   %edi
        pushl   %esi
        pushl   %ebx 
		
        movl    16(%esp), %edi
        movl    20(%esp), %esi 
        dec	      %esi
        testl   %esi, %esi
        jle     .L1
.L3:  
        movl    %edi, %eax
        movl    %edi, %ebx
        addl    %esi, %ebx
.L7:  
        movzbl  (%eax), %edx
        movzbl  1(%eax), %ecx
        cmpb    %cl, %dl
        jle     .L4
        movb    %cl, (%eax)
        movb    %dl, 1(%eax)
.L4:
        addl    $1, %eax
        cmpl    %ebx, %eax
        jne     .L7
.L6:
        subl    $1, %esi
        jne     .L3
.L1:
        popl    %ebx
        popl    %esi
        popl    %edi
        ret
		
		
# int as_puts(char *s, int n)
 .globl  as_puts
        .type   as_puts, @function
as_puts:
	push	%ebx
	movl	$4, %eax
	movl	$1, %ebx
	movl	8(%esp), %ecx
	movl	12(%esp), %edx
	int	$0x80
	pop	%ebx
	ret

# void as_exit(void)
 .globl  as_exit
        .type   as_exit, @function
as_exit:
	movl	$1, %eax
	movl	$0, %ebx
	int	$0x80


