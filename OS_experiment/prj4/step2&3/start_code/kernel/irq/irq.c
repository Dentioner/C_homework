#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
do_scheduler();
	time_elapsed += 100000; // I donot know what is a good value for time_elapsed

    
    // reset cp0_count & cp0_compare
    /*
    asm volatile(
		"mtc0	$0, CP0_COUNT\n\t"
		"li		k0, TIMER_INTERVAL\n\t"
		"mtc0	k0, CP0_COMPARE\n\t"		
	);
    */ 
    reset_timer();
    screen_reflush(); 
//	screen_clear();
//    do_scheduler();
//  	screen_reflush();  
    
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
    uint32_t int_signal;
    int_signal = cause & 0x0000ff00;
    if (int_signal == 0x8000)
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
    uint32_t ehi, elo0, elo1;
    int index1;
    
    for (index1 = 0; index1 < 32; index1++)
    {    
        set_cp0_index(index1);
        asm volatile("tlbr");
        ehi = get_cp0_entryhi();
        elo0 = get_cp0_entrylo0();
        elo1 = get_cp0_entrylo1();
        printk("i=%d, hi=%x,lo0=%x,lo1=%x\n", index1, ehi, elo0, elo1);
    }
    while(1);
}
