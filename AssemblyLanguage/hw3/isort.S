.code32
.section .data
 testdata:
   .int 6, -5, 3, 1, -8, 7, 0, 2, 4, 9
  length:
	.int 10;
.section .text
.globl _start
_start:
    subl		$8, %esp
	movl		$10, 4(%esp)
	movl		$testdata, (%esp)
	
	
	
  	call	Insert_sort
#output
	call	as_puts
#exit
	call	as_exit

 .globl  Insert_sort
        .type   Insert_sort, @function
Insert_sort:
        pushl   %edi
        pushl   %esi
        pushl   %ebx 
		
        movl    16(%esp), %edi 
        movl    20(%esp), %esi 
		dec		%esi
		testl		%esi, %esi
		jle			.L1
		
		
		movl		%edi, %eax 
		addl		$1, %eax    
		movl		%edi, %ebx
		addl		%esi, %ebx 
		addl		$1, %ebx
		movl		%eax, %ecx 
			
.L3:
		
		movl		%ecx, %edx 
		subl		$1, %edx 
		
		
.L7:
		cmpl		%edx, %edi
		jl	.L6
		movl		(%edx), %esi
		cmpl		%esi, (%ecx)
		jge	.L6
		movl		%esi, 1(%edx) 
		subl		$1, %edx		
		
		jmp	.L7
	
	
.L6:
		movl		(%ecx), %esi
		movl		%esi, 1(%edx)
		addl		$1, %ecx   #i++
		cmpl		%ecx, %ebx
		jg		.L3
		
	
.L1:
        popl    %ebx
        popl    %esi
        popl    %edi
        ret
		


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
