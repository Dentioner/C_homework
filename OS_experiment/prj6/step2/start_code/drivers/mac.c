#include "mac.h"
#include "irq.h"



#define NUM_DMA_DESC 48
queue_t recv_block_queue;
uint32_t recv_flag[PNUM] = {0};
uint32_t ch_flag;
uint32_t mac_cnt = 0;

int send_num = PNUM;
int recv_num = PNUM;

int recv_num_now = 0;
desc_t *tmp_recv = &rx_desc_list[0];

uint32_t reg_read_32(uint32_t addr)
{
    return *((uint32_t *)addr);
}
uint32_t read_register(uint32_t base, uint32_t offset)
{
    uint32_t addr = base + offset;
    uint32_t data;

    data = *(volatile uint32_t *)addr;
    return data;
}

void reg_write_32(uint32_t addr, uint32_t data)
{
    *((uint32_t *)addr) = data;
}

static void gmac_get_mac_addr(uint8_t *mac_addr)
{
    uint32_t addr;

    addr = read_register(GMAC_BASE_ADDR, GmacAddr0Low);
    mac_addr[0] = (addr >> 0) & 0x000000FF;
    mac_addr[1] = (addr >> 8) & 0x000000FF;
    mac_addr[2] = (addr >> 16) & 0x000000FF;
    mac_addr[3] = (addr >> 24) & 0x000000FF;

    addr = read_register(GMAC_BASE_ADDR, GmacAddr0High);
    mac_addr[4] = (addr >> 0) & 0x000000FF;
    mac_addr[5] = (addr >> 8) & 0x000000FF;
}

void print_tx_dscrb(mac_t *mac)
{
    uint32_t i;
    printf("send buffer mac->saddr=0x%x ", mac->saddr);
    printf("mac->saddr_phy=0x%x ", mac->saddr_phy);
    printf("send discrb mac->td_phy=0x%x\n", mac->td_phy);
#if 0
    desc_t *send=mac->td;
    for(i=0;i<mac->pnum;i++)
    {
        printf("send[%d].tdes0=0x%x ",i,send[i].tdes0);
        printf("send[%d].tdes1=0x%x ",i,send[i].tdes1);
        printf("send[%d].tdes2=0x%x ",i,send[i].tdes2);
        printf("send[%d].tdes3=0x%x ",i,send[i].tdes3);
    }
#endif
}

void print_rx_dscrb(mac_t *mac)
{
    uint32_t i;
    printf("recieve buffer add mac->daddr=0x%x ", mac->daddr);
    printf("mac->daddr_phy=0x%x ", mac->daddr_phy);
    printf("recieve discrb add mac->rd_phy=0x%x\n", mac->rd_phy);
    desc_t *recieve = (desc_t *)mac->rd;
#if 0
    for(i=0;i<mac->pnum;i++)
    {
        printf("recieve[%d].tdes0=0x%x ",i,recieve[i].tdes0);
        printf("recieve[%d].tdes1=0x%x ",i,recieve[i].tdes1);
        printf("recieve[%d].tdes2=0x%x ",i,recieve[i].tdes2);
        printf("recieve[%d].tdes3=0x%x\n",i,recieve[i].tdes3);
    }
#endif
}

static uint32_t printf_recv_buffer(uint32_t recv_buffer)
{
}
void mac_irq_handle(void)
{
    /*
    if(!queue_is_empty(&recv_block_queue))
    {
        do_unblock_one(&recv_block_queue);
    }
*/


/*******************************debug*****************************************/
    //printk("In mac_irq.");
    //while(1);

/*******************************debug*****************************************/



    if((!queue_is_empty(&recv_block_queue)))
    {
        //if(!(tmp_recv->tdes0 & DESC_OWN))
        //{
            do_unblock_one(&recv_block_queue);
        //}
        /*
        else
        {
            int index1;
            vt100_move_cursor(1, 2);
            for(index1 = 0; index1< PNUM; index1++)
            {
                if(rx_desc_list[index1].tdes0 & DESC_OWN)
                    break;
            }
            printk("[RECV TASK]still waiting recv %dth package.\n", index1);
            
        }
        */
    }

    clear_interrupt();
}

/*void register_irq_handler(int IRQn, irq_handler_t func)
{

}*/

void clear_interrupt()
{
    uint32_t data;
    data = reg_read_32(0xbfe11000 + DmaStatus);
    reg_write_32(0xbfe11000 + DmaStatus, data);
}

void irq_enable(int IRQn)
{
    reg_write_32(INT1_EN_ADDR, 0x8);
}

// WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// this function is not in kernel
void mac_recv_handle(mac_t *test_mac)  
{
    //desc_t *tmp_recv = (desc_t*)test_mac->rd;
    tmp_recv = (desc_t*)test_mac->rd;
    
    uint32_t daddr = test_mac->daddr;
    uint32_t * tmp_data;


    int index1, index2;


/*******************************debug*****************************************/
    //printf("debug:In recv handler.\n");
    //while(1);

/*******************************debug*****************************************/    





    //while(recv_num_now < recv_num)
    for(index2 = 0; index2 < PNUM; index2++)
    {
        if(!(tmp_recv->tdes0 & DESC_OWN)) // OWN = 0, received a package
        {
            /*if(tmp_recv->tdes0 & RECV_ERROR)
            {
                printf("RECV ERROR.\n");
                while(1);
            }*/

            
            tmp_data = (uint32_t *)(daddr + index2*0x400);
            //if((tmp_data[0]==0x84541b10) && (tmp_data[1]== 0xe290d683) && (tmp_data[2]== 0xd0cd15ba))
            if((tmp_data[0]==0xb57b5500) && (tmp_data[1]== 0x5a70f77d) && (tmp_data[2]== 0x0e42d30f)) // this means the package is  from pktRxTx to our kernel
            //if((tmp_data[0]==0xd7c60e00) && (tmp_data[1]== 0xfa807b77) && (tmp_data[2]== 0xef56335b)) // this means the package is  from pktRxTx to our kernel
            
            {
                sys_move_cursor(1, 4);
                for(index1 = 0; index1 < PSIZE; index1++)
                {
                    //if(tmp_data[index1] != 0)
                    //printf("%x  ", tmp_data[index1]);
                    task2_print_buffer[recv_num_now][index1] = tmp_data[index1];
                }
                task2_rdes0[recv_num_now] = tmp_recv->tdes0;
                
                recv_num_now++;
                if(recv_num_now >= PNUM)
                {
                    break;
                }
            }
        }

        
        tmp_recv = (desc_t *)((tmp_recv->tdes3) | GET_UNMAPPED_VADDR);
/*******************************debug*****************************************/
/*******************************↓↓↓↓↓*****************************************/  
        /*if(recv_num_now >= PNUM)
        {
            printf("debug:check point3.\n");
            //while(1);
            printf("index2=%d, valid num=%d\n", index2, recv_num_now);
            //do_sleep(2);    
        }*/
/*******************************↑↑↑↑↑*****************************************/
/*******************************debug*****************************************/          
        
    }
/*******************************debug*****************************************/
/*******************************↓↓↓↓↓*****************************************/
        /*if(recv_num_now >= PNUM)
        {
            printf("debug:check point4.\n");
            while(1);    
        }*/
/*******************************↑↑↑↑↑*****************************************/
/*******************************debug*****************************************/


    sys_move_cursor(1, 9);
    printf("recv valid %d packages!:", recv_num_now);

/*******************************debug*****************************************/
/*******************************↓↓↓↓↓*****************************************/  
    /*if(recv_num_now >= PNUM)
    {
        printf("debug:check point2.\n");
        while(1);    
    }*/
/*******************************↑↑↑↑↑*****************************************/
/*******************************debug*****************************************/  

}

static uint32_t printk_recv_buffer(uint32_t recv_buffer)
{
}

void set_sram_ctr()
{
    *((volatile unsigned int *)0xbfd00420) = 0x8000; /* 使能GMAC0 */
}
static void s_reset(mac_t *mac) //reset mac regs
{
    uint32_t time = 1000000;
    reg_write_32(mac->dma_addr, 0x01);

    while ((reg_read_32(mac->dma_addr) & 0x01))
    {
        reg_write_32(mac->dma_addr, 0x01);
        while (time)
        {
            time--;
        }
    };
}
void disable_interrupt_all(mac_t *mac)
{
    reg_write_32(mac->dma_addr + DmaInterrupt, DmaIntDisable);
    return;
}
void set_mac_addr(mac_t *mac)
{
    uint32_t data;
    uint8_t MacAddr[6] = {0x00, 0x55, 0x7b, 0xb5, 0x7d, 0xf7};
    uint32_t MacHigh = 0x40, MacLow = 0x44;
    data = (MacAddr[5] << 8) | MacAddr[4];
    reg_write_32(mac->mac_addr + MacHigh, data);
    data = (MacAddr[3] << 24) | (MacAddr[2] << 16) | (MacAddr[1] << 8) | MacAddr[0];
    reg_write_32(mac->mac_addr + MacLow, data);
}
uint32_t do_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr)
{
    int index1, index2, index3, index4;
    desc_t * tmp_recv = (desc_t *)rd;

    uint32_t * tmp_data;
    

    reg_write_32(DMA_BASE_ADDR + 0xc, rd_phy);
    reg_write_32(GMAC_BASE_ADDR, reg_read_32(GMAC_BASE_ADDR) | 0x4); // let mac[3] = 1
    reg_write_32(DMA_BASE_ADDR + 0x18, reg_read_32(DMA_BASE_ADDR + 0x18) | 0x02200002); // start tx, rx
    // 0x02200002 = 0000_0010_0010_0000_0000_0000_0000_0010
    // reg[25] = 1----> receive store and forward;
    // reg[21] = 1----> Transmit store and forward;
    // reg[1]  = 1----> Start receive;
    reg_write_32(DMA_BASE_ADDR + 0x1c, 0x10001 | (1 << 6));
    // 0x10001 | (1 << 6) = 0000_0000_0000_0001_0000_0000_0100_0001
    // reg[16] = 1----> normal interrupt summary enable; 
    // reg[6]  = 1----> receive interrupt enable;
    // reg[0]  = 1----> transmit interrupt enable;


    tmp_recv = (desc_t *)rd;
    for(index1 = 0; index1 < PNUM; index1++)
    {
        //rx_desc_list[index1].tdes0 |= DESC_OWN;
        //tmp_recv[index1].tdes0 |= DESC_OWN;
        tmp_recv->tdes0 |= DESC_OWN;
        tmp_recv = (desc_t *)((tmp_recv->tdes3) | GET_UNMAPPED_VADDR);
    }

    
    for (index2 = 0; index2 < recv_num; index2++)
    {
        reg_write_32(DMA_BASE_ADDR + DmaRPD, 1);
    }


    tmp_recv = (desc_t *)rd;
    index2 = 0;
    index3 = 0;
    
    return 0;
}

void do_net_send(uint32_t td, uint32_t td_phy)
{
    int index1, index2, index3, index4;
    desc_t * tmp_send;


    reg_write_32(DMA_BASE_ADDR + 0x10, td_phy);

    // MAC rx/tx enable

    reg_write_32(GMAC_BASE_ADDR, reg_read_32(GMAC_BASE_ADDR) | 0x8);                    // enable MAC-TX
    reg_write_32(DMA_BASE_ADDR + 0x18, reg_read_32(DMA_BASE_ADDR + 0x18) | 0x02202000); //0x02202002); // start tx, rx
    // 0x02202000 = 0000_0010_0010_0000_0010_0000_0000_0000
    // reg[25] = 1----> receive store and forward;
    // reg[21] = 1----> Transmit store and forward;
    // reg[13] = 1----> Start transmission;
    


    reg_write_32(DMA_BASE_ADDR + 0x1c, 0x10001 | (1 << 6));
    // 0x10001 | (1 << 6) = 0000_0000_0000_0001_0000_0000_0100_0001
    // reg[16] = 1----> normal interrupt summary enable; 
    // reg[6]  = 1----> receive interrupt enable;
    // reg[0]  = 1----> transmit interrupt enable;


    tmp_send = (desc_t *)td;

    for(index1 = 0; index1 < PNUM; index1++)
    {
        //tx_desc_list[index1].tdes0 |= DESC_OWN;
        tmp_send->tdes0 |= DESC_OWN;
        tmp_send = (desc_t *)((tmp_send->tdes3) | GET_UNMAPPED_VADDR);
    }

    for (index2 = 0; index2 < send_num; index2++)
    {

        reg_write_32(DMA_BASE_ADDR + DmaTPD, 1);
    }

}

void do_init_mac(void)
{
    mac_t test_mac;
    uint32_t i;

    test_mac.mac_addr = 0xbfe10000;
    test_mac.dma_addr = 0xbfe11000;

    test_mac.psize = PSIZE * 4; // 64bytes
    test_mac.pnum = PNUM;       // pnum

    set_sram_ctr(); /* 使能GMAC0 */
    s_reset(&test_mac);
    disable_interrupt_all(&test_mac);
    set_mac_addr(&test_mac);

    reg_write_32(INT1_CLR_ADDR, 0xffffffff);
    reg_write_32(INT1_POL_ADDR, 0xffffffff);
    reg_write_32(INT1_EDGE_ADDR, 0);
}

void do_wait_recv_package(void)
{
    do_block(&recv_block_queue);
    /*int i;
    for (i = 0; i < PNUM; i++)
    {
        if(rx_desc_list[i].tdes0 & 0x80000000)
            break;
    }
    //printk("index = %d\n", i);
    current_running->status = TASK_BLOCKED;
    current_running->block_in_queue = &recv_block_queue;
    queue_push(&recv_block_queue, current_running);
    do_scheduler();*/

}

void do_print_buffer()
{
    int index1, index2, index3;
    
    for(index1 = 0; index1 < PNUM; index1++)
    {
        index3 = 0;
        vt100_move_cursor(1, 3);
        printk("%d recv buffer, r_desc = 0x%x:\n", index1, task2_rdes0[index1]);

        vt100_move_cursor(1, 4);
        for(index2 = 0; index2 < PSIZE; index2++)
        {

            if (task2_print_buffer[index1][index2])
            {    
                printk("%x ", task2_print_buffer[index1][index2]);
                index3++;
                
            }
            if(index3%16 == 15)
            {
                printk("\n");
            }        
        }
    }
}