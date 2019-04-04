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
		
		
		imul		$4, %esi
		movl		%edi, %eax 
		addl		$4, %eax    
		movl		$9, %ebx

		movl		%eax, %ecx
			
.L3:
		
		movl		%ecx, %edx 
		subl		$4, %edx 
		movl		(%ecx), %eax
		
.L7:
		cmpl		%edi, %edx
		jl	.L6
		movl		(%edx), %esi
		cmpl		%esi, %eax
		jge	.L6
		movl		%esi, 4(%edx) 
		subl		$4, %edx		
		
		jmp	.L7
	
	
.L6:
		
		
		movl		%eax, 4(%edx)
		addl		$4, %ecx  
		subl		$1, %ebx
		cmpl		$0, %ebx
		jg		.L3
		
		
	
.L1:
        popl    %ebx
        popl    %esi
        popl    %edi
        ret




 .globl  as_exit
        .type   as_exit, @function
as_exit:
	movl	$1, %eax
	movl	$0, %ebx
	int	$0x80
