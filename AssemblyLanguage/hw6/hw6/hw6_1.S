.section .data
      iostring:   .asciz "ab1g2hA0H56po9wK78nB\n"
.section .text
.globl __start
__start:

	la $t0, iostring
        addu $t1, $0, $t0 
	addiu $t3, $0, 97
	
L1:   
	lb $t2, 0($t1)
        beq $t2, $0, L2
        nop
 
        sub  $t2, $t2, $t3
	slti $t4, $t2, 0
	bgtz $t4, L3
        
	nop        

        addiu $t2, $t2, 65
        sb $t2, 0($t1)
L3:   
	addiu $t1, $t1, 1
        j L1
	nop
L2:
        subu $t1, $t0, $t1
#output
        addiu $v0, $0, 4004
        addiu $a0, $0, 1
        addu  $a1, $zero, $t0
	addiu $a2, $0, 21
        syscall
#exit
	addiu   $v0, $0, 4001
	addiu   $a0, $0, 0
	syscall


