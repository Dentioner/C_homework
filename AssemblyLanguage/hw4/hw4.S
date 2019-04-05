.code32
.section .data
	inputdata:
		.byte 0,0,0,0,0,0,0,0,0,0
	#          0,x,1,2,3,4,5,6,7,8,\0
	enter:
		.asciz "\n"
	signsymbol:
		.asciz "-"

.section .text
.globl _start
_start:
	
#input
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
	mov	10(%ebp), %ecx #这里不是8，因为开头有0x前缀要拿掉
	mov  14(%ebp), %ebx  #后4位，也就是低4位
	
	movb	$'0', 0(%esp)
	movb	$'x', 1(%esp)
	movb	$0, 10(%esp)																																																																																							

	
	mov		%ecx, %edx 
	#shrl		$12,   %edx #这个操作使得edx的最高位跑到dl上面去
	andb	$0xf, %dl  #不用右移，因为寄存器的存储方向和内存是反过来的
	cmpb	$8, %dl	#比8大说明是负数补码
	jl		normal
	mov		$1, %edi		
#从补码返回原码，直接改内存中inputdata的值
#这一段里面ebx ecx暂时没用了
	mov		%ebp, %eax
	add		$10, %eax #eax现在是inputdata的指针了
	mov		$8, %ecx #循环计数器
	mov		$0, %esi
	
loooop:
	movb	(%eax), %bl
	cmpb	$0x39, %bl
	jna		L1
	jmp		L2
L1:  #将数字字符转为真实数字	
	subb		$0x30, %bl
	jmp		L3
L2: #将字母字符转为真实数字
	subb		$87, %bl
	jmp		L3
L3: #统一取异或值
	xorb		$0xf, %bl
	cmp		$7,	%esi
	jne		L3_5
	addb		$1, %bl #原补码互相转换最后一位要+1
L3_5:
	cmpb	$9, %bl
	jna		L4
	jmp		L5
	
L4: #将≤9的数字转为数字字符
	addb	$0x30, %bl
	jmp		L6
L5: #将＞9的数字转为字母
	addb	$87, %bl
	jmp		L6
L6:
	movb	%bl, (%eax)
	inc		%eax
	inc		%esi

loop loooop

	mov	10(%ebp), %ecx #这里不是8，因为开头有0x前缀要拿掉
	mov  14(%ebp), %ebx  #后4位，也就是低4位




	
normal:
	movl		%ecx, 2(%esp)
	movl		%ebx,  6(%esp)
	
	cmp		$1, %edi
	jne		printmain
	addl		$1, 9(%esp)#补码返回原码的时候取完反之后尾数还要+1 #←要改
#print

printsign:
	movl	$4, %eax
	movl	$1, %ebx
	movl	$signsymbol, %ecx
	movl	$1, %edx
	int	$0x80

#下面打印部分需要改成循环打印
printmain:	
	mov	$4, %eax
	mov	$1, %ebx
	mov	%esp, %ecx
	mov	$10, %edx
	int	$0x80




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
	
	
	
