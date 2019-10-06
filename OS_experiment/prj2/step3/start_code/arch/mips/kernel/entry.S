#include "asm.h"

.equ    KERNEL, 0
.equ    USER,   156


# according to https://blog.csdn.net/lanwanjunxixihaha/article/details/46518563
# cli & sti are interrupt enable/disable instructions in x86
# so here we can use these macro function to accomplish these instructions in MIPS32
.macro STI
    mfc0    k0, CP0_STATUS
    ori     k0, k0, 0x1         # just let IE = 1? is it OK? what about EXL & ERL?
    mtc0    k0, CP0_STATUS
.endm

.macro CLI

    mfc0    k0, CP0_STATUS
    #andi    k0, k0, 0xfffffffe  # just let IE = 0
    #and     k0, 0xfffffffe
    li      k1, 0xfffffffe
    and     k0, k0, k1
    mtc0    k0, CP0_STATUS
.endm

LEAF(asm_start)
    mtc0    zero, CP0_STATUS
    mtc0    zero, CP0_WATCHLO
    mtc0    zero, CP0_WATCHHI

    mfc0    t0, CP0_CONFIG
    and     t0, ~0x7
    ori     t0, 0x2
    mtc0    t0, CP0_CONFIG
    
    jr  ra
END(asm_start)

# I find that I cannot use asm in .c file, so that I use LEAF&END to build small function
# reference: https://blog.csdn.net/ivan240/article/details/5543985

LEAF(get_cp0_status)
    mfc0    v0, CP0_STATUS
    jr      ra
END(get_cp0_status)

LEAF(disable_all_interrupt)
    CLI
    jr      ra
END(disable_all_interrupt)

LEAF(open_all_interrupt)
    STI
    jr      ra
END(open_all_interrupt)

LEAF(reset_timer)
    mtc0    $0, CP0_COUNT
#    li      k0, TIMER_INTERVAL
    li      k0, 1500000
    mtc0    k0, CP0_COMPARE
    jr      ra
END(reset_timer)

LEAF(set_cp0_status)
    mtc0    a0, CP0_STATUS
    jr      ra
END(set_cp0_status)

LEAF(get_count)
	mfc0	v0, CP0_COUNT
	jr	ra
END(get_count)

LEAF(get_compare)
	mfc0	v0, CP0_COMPARE
	jr	ra
END(get_compare)

.macro SAVE_CONTEXT offset
    // TODO save context
    # code with "#" is pseudo code
    .set noat # .set noat prevents the assembler from translating assembly code into binary sequences relying on the at/$1 register.
    lw    $26, current_running  #k0
    addi k0, k0, \offset      #the reason of using '\':https://www.cnblogs.com/zjzsky/p/3534109.html
#no sw $0 xxx,  because 0(k0) is $0 register
    sw    $1, 4(k0)    #at
    sw    $2, 8(k0)    #v0
    sw    $3, 12(k0)   #v1
    sw    $4, 16(k0)   #a0
    sw    $5, 20(k0)   #a1
    sw    $6, 24(k0)   #a2
    sw    $7, 28(k0)   #a3
    sw    $8, 32(k0)   #t0
    sw    $9, 36(k0)   #t1
    sw    $10, 40(k0)  #t2
    sw    $11, 44(k0)  #t3
    sw    $12, 48(k0)  #t4
    sw    $13, 52(k0)  #t5
    sw    $14, 56(k0)  #t6
    sw    $15, 60(k0)  #t7
    sw    $16, 64(k0)  #s0
    sw    $17, 68(k0)  #s1
    sw    $18, 72(k0)  #s2
    sw    $19, 76(k0)  #s3
    sw    $20, 80(k0)  #s4
    sw    $21, 84(k0)  #s5
    sw    $22, 88(k0)  #s6
    sw    $23, 92(k0)  #s7
    sw    $24, 96(k0)  #t8
    sw    $25, 100(k0) #t9
#    sw    k0, 104(k0) #k0       #in regs.h saying that k0 and k1 do not need to save
#    sw    $27, 108(k0) #k1
    sw    $28, 112(k0) #gp
    sw    $29, 116(k0) #sp
    sw    $30, 120(k0) #fp
    sw    $31, 124(k0) #ra

    # c0-registers
    # because mfc0 is used with 2 registers 
    # so I cannot use it directly to move data between MEM and REG
    mfc0    k1, CP0_STATUS # cp0_status
    sw      k1, 128(k0)    

    mfhi    k1  #hi
    sw      k1, 132(k0)

    mflo    k1  #lo
    sw      k1, 136(k0)

    mfc0    k1, CP0_BADVADDR    #cp0_bad_vaddr
    sw      k1, 140(k0)

    mfc0    k1, CP0_CAUSE   #cp0_cause
    sw      k1, 144(k0)

    mfc0    k1, CP0_EPC     #cp0_epc
    sw      k1, 148(k0)

    # what about PC?

    
.endm

.macro RESTORE_CONTEXT offset
    // TODO restore context
    # code with "#" is pseudo code
#no lw $0 xxx,  because 0(k0) is $0 register
    .set noat   # .set noat prevents the assembler from translating assembly code into binary sequences relying on the at/$1 register.
    lw    k0, current_running  #k0
    addi  k0, k0, \offset     #the reason of using '\':https://www.cnblogs.com/zjzsky/p/3534109.html

    lw    $1, 4(k0)    #at
    lw    $2, 8(k0)    #v0
    lw    $3, 12(k0)   #v1
    lw    $4, 16(k0)   #a0
    lw    $5, 20(k0)   #a1
    lw    $6, 24(k0)   #a2
    lw    $7, 28(k0)   #a3
    lw    $8, 32(k0)   #t0
    lw    $9, 36(k0)   #t1
    lw    $10, 40(k0)  #t2
    lw    $11, 44(k0)  #t3
    lw    $12, 48(k0)  #t4
    lw    $13, 52(k0)  #t5
    lw    $14, 56(k0)  #t6
    lw    $15, 60(k0)  #t7
    lw    $16, 64(k0)  #s0
    lw    $17, 68(k0)  #s1
    lw    $18, 72(k0)  #s2
    lw    $19, 76(k0)  #s3
    lw    $20, 80(k0)  #s4
    lw    $21, 84(k0)  #s5
    lw    $22, 88(k0)  #s6
    lw    $23, 92(k0)  #s7
    lw    $24, 96(k0)  #t8
    lw    $25, 100(k0) #t9
#    lw    k0, 104(k0) #k0       #in regs.h saying that k0 and k1 do not need to save
#    lw    $27, 108(k0) #k1
    lw    $28, 112(k0) #gp
    lw    $29, 116(k0) #sp
    lw    $30, 120(k0) #fp
    lw    $31, 124(k0) #ra

    # c0-registers
    # because mtc0 is used with 2 registers 
    # so I cannot use it directly to move data between MEM and REG
    lw      k1, 128(k0)
    mtc0    k1, CP0_STATUS # cp0_status
        
    lw      k1, 132(k0)
    mthi    k1  #hi
   
    lw      k1, 136(k0)
    mtlo    k1  #lo
    
    lw      k1, 140(k0)
    mtc0    k1, CP0_BADVADDR    #cp0_bad_vaddr
    
    lw      k1, 144(k0)
    mtc0    k1, CP0_CAUSE   #cp0_cause
    
    lw      k1, 148(k0)
    mtc0    k1, CP0_EPC     #cp0_epc


    # what about PC?

.endm

NESTED(do_scheduler, 0, ra)
    SAVE_CONTEXT(KERNEL)
    jal     scheduler
    RESTORE_CONTEXT(KERNEL)
    jr      ra
END(do_scheduler)

.global exception_handler_begin
.global exception_handler_end

NESTED(exception_handler_entry, 0, sp)   
exception_handler_begin:
    // TODO close interrupt
    // jmp exception_handler[i] which decided by CP0_CAUSE
    // Leve2 exception Handler.
    
    #   close interrupt
    CLI
    
    #   save context
    SAVE_CONTEXT(USER)
    
    #   jump to handle_xxx
    mfc0    k0, CP0_CAUSE           # read CP0_CAUSE register
    andi    k0, k0, CAUSE_EXCCODE   # only extract the exccode from the whole register
LABEL_INTERRUPT:
    xor     k1, k1, k1              # let k1 = 0
    bne     k0, k1, LABEL_SYSCALL
    nop
    la      k1, handle_int
    jr      k1
    nop
LABEL_SYSCALL:                      # in part3 we do not accomplish it 
    li      k1, 0x20                # 0_01000_00 <= SYS = 8, because ANDI let k0 become 0_xxxxx_00
    bne     k0, k1, LABEL_OTHER
    nop
    la      k1, handle_syscall
    jr      k1
    nop
LABEL_OTHER:                        # in part3 we do not accomplish it
    la      k1, handle_other
    jr      k1
    nop
exception_handler_end:
END(exception_handler_entry)

NESTED(handle_int, 0, sp)
    // interrupt handler
    // Leve3 exception Handler.
    # 1.jump to interrupt_helper
    mfc0    a0, CP0_STATUS
    mfc0    a1, CP0_CAUSE
    # void interrupt_helper(uint32_t status, uint32_t cause)
    addi    sp, sp, -8
    jal     interrupt_helper
    addi    sp, sp, 8

    
    # 2.restore the context
    RESTORE_CONTEXT(USER)
    
    # 3.open interrupt
    STI

    eret
END(handle_int)

NESTED(handle_syscall, 0, sp)
    // system call handler
END(handle_syscall)

NESTED(handle_other, 0, sp)
    // other exception handler
END(handle_other)


