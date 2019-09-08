.section .data
output1:
        .asciz "sum = "
enter:
        .asciz "\n"
inttt:
	.int 1551

.section .text
        .globl __start
__start:
        li    $t0, 5050
	
	li	$v0, 1
#	move	$a0, $t0
	lw	$a0, inttt
	syscall

#	li	$v0,10
#	syscall

	addiu   $v0, $0, 4001
	addiu   $a0, $0, 0
	syscall

