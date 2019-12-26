#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "queue.h"
#include "../drivers/mac.h"

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


    if((!queue_is_empty(&recv_block_queue)))
    {
        /*if(!(tmp_recv->tdes0 & DESC_OWN))
        {
            do_unblock_one(&recv_block_queue);
        }*/
        //else
        //{
            int index1;
            vt100_move_cursor(1, 2);
            for(index1 = 0; index1< PNUM; index1++)
            {
                if(rx_desc_list[index1].tdes0 & DESC_OWN)
                    break;
            }
            printk("[RECV TASK]still waiting recv %dth package.\n", index1);
            
            
/*******************************debug*****************************************/
            /*printk("valid now: %d\n", recv_num_now);


            for(index1 = 0; index1 < PNUM; index1++)
            {
                printk("%x+%x ", rx_desc_list[index1].tdes0, (rx_desc_list[index1].tdes1)>>31);
                if(index1%10 == 9)
                {
                    printk("\n");
                }
            }
            
            printk("\n%x\n", rx_desc_list[PNUM-1].tdes1);*/
/*******************************debug*****************************************/
        //}
    }



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
