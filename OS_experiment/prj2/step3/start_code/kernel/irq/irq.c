#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
    time_elapsed += 10000000; // ÔÝ¶¨Îª1000,0000
    
    // reset cp0_count & cp0_compare
    asm volatile(
		"mtc0	$0, CP0_COUNT\n\t"
		"li		k0, TIMER_INTERVAL\n\t"
		"mtc0	k0, CP0_COMPARE\n\t"		
	);
    do_scheduler();
    
    
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
    uint32_t int_signal;
    int_signal = cause & 0x0000ff00;
    if (int_signal == 0x80)
        irq_timer();
    else
    {
        printk("error signal.\n");
        while(1);
    }
    
}

void other_exception_handler()
{
    // TODO other exception handler
}