Inthandler:
	pushw	%ax
	pushw	%bx
	pushw	%dx 
1:
	movb	(%si), %al
	movb	1(%si), %bl #bl saves the following char  
	test	%al, %al 
	jz	1f
	subb	$'0', %al
	cmp	$9, %al 
	ja	2f
	jmp	m
back:
	dec	%si #if we do not add this instruction, char in bl may also be a number but we just skip it
2:
	inc	%si
	jmp	1b
1:
	popw	%dx
	popw	%bx
	popw	%ax
	iret

m:
	mov	%si, %dx
m2:
	movb	%bl, (%dx)
	inc	%dx
	movb	(%dx), %al
	movb	1(%dx), %bl
	test	%al, %al
	jz	back
	jmp	m2
