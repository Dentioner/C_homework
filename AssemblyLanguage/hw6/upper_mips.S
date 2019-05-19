#upper_mips.S, made by xuyb
.section .data
    iostring: .asciz "ab1g2hA0H56po9wK78nB\n"
.section .text
    .globl __start
__start:
    lui     $t0, %hi(iostring)
    addiu   $t0, $t0, %lo(iostring)
    addu    $a1, $zero, $t0
    
L1:    
    lb      $t1, 0($t0)
    beq     $t1, $zero, L3
    sub     $t1, $t1, 97
    slti    $t2, $t1, 0
    bgtz    $t2, L2
    addiu   $t1, $t1, 65      
    sb      $t1, 0($t0)
L2:    
    addiu   $t0, $t0, 1    
    j       L1
    
L3:
#output
    addiu   $v0, $zero, 4004
    addiu   $a0, $zero, 1
    addiu   $a2, $zero, 21
    syscall
    
#exit
    addiu   $v0, $zero, 4001
    addiu   $a0, $zero, 0
    syscall
