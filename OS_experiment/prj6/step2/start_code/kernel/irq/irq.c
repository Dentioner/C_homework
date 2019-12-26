#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "queue.h"
#include "../drivers/mac.h"

extern uint32_t current_line;
uint32_t backup_y;

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
    if (current_running->pid == 1)
        backup_y = current_line;

do_scheduler();
	time_elapsed += 100000; // I donot know what is a good value for time_elapsed

    reset_timer();
    screen_reflush(); 
//	screen_clear();
//    do_scheduler();
//  	screen_reflush();  

    if(current_running->pid == 1)
        current_line = backup_y;
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
    uint32_t int_signal;
    int_signal = status & cause & 0x0000ff00;

/*******************************debug*****************************************/
    //vt100_move_cursor(1, 13);
    //printk("status=%x,cause=%x,signal=%x\n", status, cause, int_signal);
/*******************************debug*****************************************/
    if (int_signal == 0x8000)
        irq_timer();
    else if(int_signal == 0x0800 &&(reg_read_32(INT1_SR_ADDR)) & 0x8)
    {
        mac_irq_handle();
    }
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
    int index1, index2;
    vt100_move_cursor(1, 16);
    /*for (index1 = 0; index1 < 32; index1++)
    {    
        set_cp0_index(index1);
        asm volatile("tlbr");
        ehi = get_cp0_entryhi();
        elo0 = get_cp0_entrylo0();
        elo1 = get_cp0_entrylo1();
        printk("i=%d, hi=%x,lo0=%x,lo1=%x\n", index1, ehi, elo0, elo1);
    }*/

    //vt100_move_cursor(1, 16);
    printk("cause=%x\n", (current_running->user_context.cp0_cause));
    printk("exccode=%x\n", (current_running->user_context.cp0_cause)>>2);
    printk("badvaddr=%x\n", get_cp0_badvaddr());
    printk("epc=%x\n", current_running->user_context.cp0_epc);


/*******************************debug*****************************************/
    /*for(index2 = 0; index2 < PNUM; index2++)
    {
        if(tmp_recv == &rx_desc_list[index2])
        break;
    }
    printk("index2=%d, tmp_recv=%x\n", index2, tmp_recv);*/

    /*for (index2 = 0; index2<PNUM; index2++)
    {
        printk("&rx[%d]=%x,tdes3=%x\n", index2, &rx_desc_list[index2], (rx_desc_list[index2].tdes3|GET_UNMAPPED_VADDR));
    }*/
/*******************************debug*****************************************/
    while(1);
}
