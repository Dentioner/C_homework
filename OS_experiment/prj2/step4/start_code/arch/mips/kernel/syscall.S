#include "asm.h"

LEAF(invoke_syscall)
# int invoke_syscall(int, int, int, int);
# for instance:
# invoke_syscall(SYSCALL_SLEEP, time, IGNORE, IGNORE);
# invoke_syscall(SYSCALL_UNBLOCK_ONE, (int)queue, IGNORE, IGNORE);
# so, a0 = syscall_name, a1 & a2 & a3 are syscall_parameters 
    // TODO syscall entry
# syscall also use a0~a3 to pass parameters
# so we should modify a0~a3
    add		v0, a0, $0		# v0 <= a0 
    add		a0, a1, $0		# a0 <= a1
    add		a1, a2, $0		# a1 <= a2
    add		a2, a3, $0      # a2 <= a3

    syscall

    jr		ra				
    
END(invoke_syscall)