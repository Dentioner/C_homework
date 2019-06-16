Inthandler:
	pushw	%ax
	pushw	%bx 
1:
	movb	(%si), %al
	movb	1(%si), %bl #bl saves the following char  
	test	%al, %al 
	jz	1f
	subb	$'0', %al
	cmp	$9, %al 
	jae	2f
	movb	%bl, %al # move the following char to the current position
	movb	%al, (%si)
	dec	%si #if we do not add this instruction, char in bl may also be a number but we just skip it
2:
	inc	%si
	jmp	1b
1:
	popw	%bx
	popw	%ax
	iret
