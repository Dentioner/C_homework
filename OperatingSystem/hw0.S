.section .data
output1:
	.asciz "sum = "
enter:
	.asciz "\n"
sum:
	.asciz "0000"

.section .text
	.globl __start
__start:
	addi	$t0, $0, 1
	xor	$t1, $t1, $t1
	addi	$t2, $0, 100
L1:
	add	$t1, $t1, $t0
	addi	$t0, $t0, 1
	bge	$t2, $t0, L1

	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1
	la	$a1, output1
	addiu	$a2, $0, 6
	syscall

#	addiu	$v0, $0, 4004
#	addiu	$a0, $0, 1
#	add	$a1, $0, $t1
#	addiu	$a2, $0, 6
#	syscall

#	li	$v0, 1
#	move	$a0, $t1
#	syscall
	la	$t7, sum

	xor	$t3, $t3, $t3
	addi	$t4, $0, -1
	sub	$t5, $t1, 1000
	bltz	$t5, get_num2
	nop

get_num1:
	addiu	$t4, $t4, 1
	addiu	$t3, $t3, 1000
	sub	$t5, $t3, $t1
	blez	$t5, get_num1
	nop

end_get_num1:	
	addiu	$t1, $t1, 1000
	sub	$t1, $t1, $t3
	addiu	$t4, $t4, 0x30
	sb	$t4, 0($t7)
	
	xor	$t3, $t3, $t3
	addi	$t4, $0, -1
	sub	$t5, $t1, 100
	bltz	$t5, get_num3
	nop

get_num2:
	addiu	$t4, $t4, 1
	addiu	$t3, $t3, 100
	sub	$t5, $t3, $t1
	blez	$t5, get_num2
	nop

end_get_num2:
	addiu	$t1, $t1, 100
	sub	$t1, $t1, $t3
	addiu	$t4, $t4, 0x30
	sb	$t4, 1($t7)

	xor	$t3, $t3, $t3
	addi	$t4, $0, -1
	sub	$t5, $t1, 10
	bltz	$t5, get_num4
	nop

get_num3:
	addiu	$t4, $t4, 1
	addiu	$t3, $t3, 10
	sub	$t5, $t3, $t1
	blez	$t5, get_num3
	nop

end_get_num3:
	addiu	$t1, $t1, 10
	sub	$t1, $t1, $t3
	addiu	$t4, $t4, 0x30
	sb	$t4, 2($t7)

	xor	$t3, $t3, $t3
	addi	$t4, $0, -1
	sub	$t5, $t1, 1
	bltz	$t5, printsum
	nop

get_num4:
	addiu	$t4, $t4, 1
	addiu	$t3, $t3, 1
	sub	$t5, $t3, $t1
	blez	$t5, get_num4
	nop

end_get_num4:
	addiu	$t1, $t1, 1
	sub	$t1, $t1, $t3
	addiu	$t4, $t4, 0x30
	sb	$t4, 3($t7)


printsum:
	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1
	la	$a1, sum
	addiu	$a2, $0, 4
	syscall



printenter:
	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1
	la	$a1, enter
	addiu	$a2, $0, 1
	syscall

	addiu	$v0, $0, 4001
	addiu	$a0, $0, 0
	syscall


