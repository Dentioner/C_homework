#insert_sort.S
.code32
.section .data
	testdata: 	.int 6, -5, 3, 1, -8, 7, 0, 2, 4, 9
.section .text
.globl _start
_start:
	subl	$8, %esp
	movl	$10, 4(%esp)
	movl	$testdata, (%esp)
	call	insert_sort
	nop
	
#output


#exit
	call	as_exit


#insert_sort:
.globl	insert_sort
	.type 	insert_sort, @function 	
insert_sort:
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	
	movl	16(%esp), %eax	
	movl	20(%esp), %ebx	
	movl	$1, %edi

.L1:	
	cmpl	%edi, %ebx
	jle		.L4
	
	movl 	%edi, %esi

.L2:	
	testl	%esi, %esi
	jz		.L3
	
	leal	(%eax, %esi, 4), %eax
	movl	(%eax), %ecx
	movl	-4(%eax), %edx
	
	cmpl	%edx, %ecx
	jge		.L3
	movl	%ecx, -4(%eax)
	movl	%edx, (%eax)
	
	dec		%esi
	movl	16(%esp), %eax
	jmp		.L2

.L3:
	inc 	%edi
	movl	16(%esp), %eax	
	jmp		.L1

.L4:
	popl		%ebx
	popl		%esi
	popl		%edi	
	ret	
	
	
#as_exit:
.globl as_exit
	.type	as_exit, @function
as_exit:
	movl	$1, %eax
	movl	$0, %ebx
	int		$0x80
	ret

