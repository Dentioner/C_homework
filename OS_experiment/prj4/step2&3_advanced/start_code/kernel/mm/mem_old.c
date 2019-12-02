#include "mm.h"
#include "sched.h"
//TODO:Finish memory management functions here refer to mm.h and add any functions you need.

#define RefillCtrl 0x16 // this is used for entrylo0 & entrylo1, not for PTE
// c = 3'b010, d = 1'b1, v = 1'b1, g = 1'b0



int page_fault_handler(uint32_t tmp_vpn)
{
    int index1, index2;

    for(index1 = 0; index1 < PGTABLE_NUM_ALL; index1++)
    {
        if((page_table[index1].ctrl & PTE_V) == 0) // want to find an invalid pte to be replaced
        {
            break;
        }
    }

    if(index1 == PGTABLE_NUM_ALL)
    {
        printk("Error. Don't have enough pte space.\n");
        while(1);
    }

    for(index2 = 0; index2 < PAGEFRAME_NUM; index2++)
    {
        if(page_frame_table[index2].used == 0)
        {
            break;
        }
    }

    if(index2 == PAGEFRAME_NUM)
    {
        printk("Error. Don't have enough page frame to allocate.\n");
        while(1);
    }

    page_table[index1].pfn = index2 + PFN_BASE;
    page_table[index1 + 1].pfn = index2 + 1 + PFN_BASE;

    page_table[index1].vpn = tmp_vpn;
    page_table[index1 + 1].vpn = tmp_vpn + 1;

    page_table[index1].ctrl |= PTE_V;
    page_table[index1 + 1].ctrl |= PTE_V;

    page_table[index1].pte_pid = current_running->pid;
    page_table[index1 + 1].pte_pid = current_running->pid;
    
    
    page_frame_table[index2].used = 1;
    page_frame_table[index2 + 1].used = 1;
    page_frame_table[index2].pfn_pid = current_running->pid;
    page_frame_table[index2 + 1].pfn_pid = current_running->pid;

    return index1;
}

void do_TLB_Refill()//void tlb_ex_helper()
{
    /*
    uint32_t vpn, vpn2;
    // 1111_1111_1111_1111_1111_0000_0000_0000 => 0xfffff000
    vpn = ((current_running->user_context.cp0_badvaddr) & 0xfffff000) >> 12;
    vpn2 = ((current_running->user_context.cp0_badvaddr) & 0xffffe000) >> 12;
    */
    // cp0_context:
    //  |  31~23|   22~4|3~0|
    //  |PTEBASE|BadVPN2| 0 |

    uint32_t tmp_context, badvpn2;
    uint32_t tmp_entryhi, asid;
    uint32_t tmp_index, p_index;
    uint32_t tmp_badvaddr;
    uint32_t tmp_entrylo0, tmp_entrylo1;
    pte_t *tmp_pte;

    
    static uint8_t tlb_entry = 0;

    int index1, index3;
    //tmp_context = get_cp0_context();
    //badvpn2 = (tmp_context & 0x007ffff0) >> 4;
    tmp_badvaddr = get_cp0_badvaddr();
    badvpn2 = (tmp_badvaddr & 0xffffe000) >> 12; // vpn2需要额外抹掉第12位

    tmp_entryhi = get_cp0_entryhi();
    asid = tmp_entryhi & 0xff;

    tmp_entrylo0 = get_cp0_entrylo0();
    tmp_entrylo1 = get_cp0_entrylo1();
    

    if(asid != current_running->pid && (asid != 0))
    {
        vt100_move_cursor(1, 1);
        printk("Error. asid = %d neq pid = %d                             \n", asid, current_running->pid);
        //test:
        printk("hi = %x ", tmp_entryhi);
	printk("badvpn = %x ", badvpn2);
        printk("type is %x ", current_running->user_context.cp0_cause>>2);
        printk("lo0 = %x, lo1 = %x\n", tmp_entrylo0, tmp_entrylo1);
        //while(1);//test

        do_exit();
    }

    //set_cp0_entryhi((badvpn2 << 13) | asid);
    set_cp0_entryhi((badvpn2 << 13) | current_running->pid);
    
    asm volatile("tlbp");
    
    tmp_index = get_cp0_index();
    p_index = tmp_index >> 31;


    // TODO: task3
    // detect whether page fault exists

    set_cp0_entryhi((tmp_entryhi & 0xffffe000) | current_running->pid);

    for(index1 = 0; index1 < PGTABLE_NUM_ALL; index1++)
    {
        if((page_table[index1].vpn == badvpn2) && (page_table[index1].ctrl & PTE_V) && (page_table[index1].pte_pid == current_running->pid))
        {
            break;
        }
    }

    if(index1 == PGTABLE_NUM_ALL) // 说明没有找到有效的页表，属于page fault
    {
        index3 = page_fault_handler(badvpn2);
        // badvpn2一定是偶数，因此上面的函数返回值一定是偶数页的索引，注意。
        tmp_pte = &page_table[index3];
    }
    else
    {
        tmp_pte = &page_table[index1];
    }

    //处理完page fault之后还得回来处理原来的invalid 或 refill
    set_cp0_entrylo0(((tmp_pte->pfn)<<6) | RefillCtrl);
    set_cp0_entrylo1(((tmp_pte->pfn + 1)<<6) | RefillCtrl);
    set_cp0_pagemask(0);

    if(p_index) // TLB refill
    {
        tlb_entry = (tlb_entry + 1)%32;
        set_cp0_index(tlb_entry);
    }
    /*
    else        // TLB invalid
    {
    }
    */
    // it seems that TLB_invalid does not need to set cp0_index since it can find its TLB entry after instruction 'tlbp'
    
    asm volatile("tlbwi");


    
}