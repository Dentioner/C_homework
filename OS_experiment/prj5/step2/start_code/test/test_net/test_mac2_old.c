#include "mac.h"
#include "irq.h"
#include "type.h"
#include "screen.h"
#include "syscall.h"
#include "sched.h"
#include "test4.h"

desc_t *send_desc;
desc_t *receive_desc;
uint32_t cnt = 1; //record the time of iqr_mac
//uint32_t buffer[PSIZE] = {0x00040045, 0x00000100, 0x5d911120, 0x0101a8c0, 0xfb0000e0, 0xe914e914, 0x00000801,0x45000400, 0x00010000, 0x2011915d, 0xc0a80101, 0xe00000fb, 0x14e914e9, 0x01080000};
uint32_t buffer[PSIZE] = {0xffffffff, 0x5500ffff, 0xf77db57d, 0x00450008, 0x0000d400, 0x11ff0040, 0xa8c073d8, 0x00e00101, 0xe914fb00, 0x0004e914, 0x0000, 0x005e0001, 0x2300fb00, 0x84b7f28b, 0x00450008, 0x0000d400, 0x11ff0040, 0xa8c073d8, 0x00e00101, 0xe914fb00, 0x0801e914, 0x0000};

/**
 * Clears all the pending interrupts.
 * If the Dma status register is read then all the interrupts gets cleared
 * @param[in] pointer to synopGMACdevice.
 * \return returns void.
 * you will use it in task3
 */
void clear_interrupt()
{
    uint32_t data;
    data = reg_read_32(0xbfe11000 + DmaStatus);
    reg_write_32(0xbfe11000 + DmaStatus, data);
}

static void mac_send_desc_init(mac_t *mac)
{
        int index1, index2;

    for (index1 = 0; index1 < PNUM -1; index1++)
    {

        for (index2 = 0; index2 < PSIZE; index2++)
        {
            send_package[index1][index2] = 0;
        }
        
        tx_desc_list[index1].tdes0 = 0;
        // let [24] = 1, [10:0] = sizeof(buffer1)
        tx_desc_list[index1].tdes1 = TX_HAS_LINK | BUFFER_SIZE;
        // save addr of buffer1
        tx_desc_list[index1].tdes2 = ((uint32_t)&(send_package[index1])) & GET_UNMAPPED_PADDR;
        // save the addr of next link node
        tx_desc_list[index1].tdes3 = ((uint32_t)&(tx_desc_list[index1+ 1])) & GET_UNMAPPED_PADDR;
    

    }

    for (index2 = 0; index2 < PSIZE; index2++)
    {
        send_package[PNUM -1][index2] = 0;
    }

    tx_desc_list[PNUM -1].tdes0 = 0;
    
    tx_desc_list[PNUM -1].tdes1 = TX_HAS_LINK | BUFFER_SIZE | TX_LINK_END;
    tx_desc_list[PNUM -1].tdes2 = ((uint32_t)&(send_package[PNUM-1])) & GET_UNMAPPED_PADDR;
    tx_desc_list[PNUM -1].tdes3 = ((uint32_t)&(tx_desc_list[0])) & GET_UNMAPPED_PADDR;




//    mac->psize = PSIZE;
//    mac->pnum = PNUM;
//    mac->mac_addr = 
//    mac->dma_addr =
    mac->saddr = (uint32_t)&(send_package[0]);
//    mac->daddr = 
    mac->saddr_phy = ((uint32_t)&send_package[0]) & GET_UNMAPPED_PADDR;
//    mac->daddr_phy =
    mac->td = (uint32_t)&(tx_desc_list[0]);
//    mac->rd = 
    mac->td_phy = ((uint32_t)&(tx_desc_list[0])) & GET_UNMAPPED_PADDR;
//    mac->rd_phy = 

    for (index1 = 0; index1 < PNUM; index1++)
    {
        for(index2 = 0; index2 < PSIZE; index2++)
        {
            send_package[index1][index2] = buffer[index2];
        }
    }
}

static void mac_recv_desc_init(mac_t *mac)
{
    int index1, index2;

    for (index1 = 0; index1 < PNUM -1; index1++)
    {
        for(index2 = 0; index2 < PSIZE; index2 ++)
        {
            recv_package[index1][index2] = 0;
        }

        rx_desc_list[index1].tdes0 = 0;
        // let [24] = 1, [10:0] = sizeof(buffer1)
        rx_desc_list[index1].tdes1 = RX_HAS_LINK | BUFFER_SIZE;
        // save addr of buffer1
        rx_desc_list[index1].tdes2 = ((uint32_t)&(recv_package[index1])) & GET_UNMAPPED_PADDR;
        // save the addr of next link node
        rx_desc_list[index1].tdes3 = ((uint32_t)&(rx_desc_list[index1+ 1])) & GET_UNMAPPED_PADDR;
        

    }

    for(index2 = 0; index2 < PSIZE; index2 ++)
    {
        recv_package[PNUM-1][index2] = 0;
    }

    rx_desc_list[PNUM-1].tdes0 = 0;
    rx_desc_list[PNUM-1].tdes1 = RX_HAS_LINK | RX_LINK_END | BUFFER_SIZE;
    rx_desc_list[PNUM-1].tdes2 = ((uint32_t)&(recv_package[PNUM-1])) & GET_UNMAPPED_PADDR;
    rx_desc_list[PNUM-1].tdes3 = ((uint32_t)&(rx_desc_list[0])) & GET_UNMAPPED_PADDR;


//    mac->psize = PSIZE;
//    mac->pnum = PNUM;
//    mac->mac_addr = 
//    mac->dma_addr =
//    mac->saddr = 
    mac->daddr = (uint32_t)&(recv_package[0]);
//    mac->saddr_phy = 
    mac->daddr_phy = ((uint32_t)&(recv_package[0])) & GET_UNMAPPED_PADDR;
//    mac->td = 
    mac->rd = (uint32_t)&(rx_desc_list[0]);
//    mac->td_phy = 
    mac->rd_phy = ((uint32_t)&(rx_desc_list[0])) & GET_UNMAPPED_PADDR;
}

static uint32_t printf_recv_buffer(uint32_t recv_buffer)
{
}
static void mii_dul_force(mac_t *mac)
{
    reg_write_32(mac->dma_addr, 0x80); //?s
                                       //   reg_write_32(mac->dma_addr, 0x400);
    uint32_t conf = 0xc800;            //0x0080cc00;

    // loopback, 100M
    reg_write_32(mac->mac_addr, reg_read_32(mac->mac_addr) | (conf) | (1 << 8));
    //enable recieve all
    reg_write_32(mac->mac_addr + 0x4, reg_read_32(mac->mac_addr + 0x4) | 0x80000001);
}

void dma_control_init(mac_t *mac, uint32_t init_value)
{
    reg_write_32(mac->dma_addr + DmaControl, init_value);
    return;
}

void mac_send_task()
{

    mac_t test_mac;
    uint32_t i;
    uint32_t print_location = 2;

    test_mac.mac_addr = 0xbfe10000;
    test_mac.dma_addr = 0xbfe11000;

    test_mac.psize = PSIZE * 4; // 64bytes
    test_mac.pnum = PNUM;       // pnum

    mac_send_desc_init(&test_mac);

    dma_control_init(&test_mac, DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128);
    clear_interrupt(&test_mac);

    mii_dul_force(&test_mac);

    // register_irq_handler(LS1C_MAC_IRQ, mac_irq_handle);

    // irq_enable(LS1C_MAC_IRQ);
    sys_move_cursor(1, print_location);
    printf("> [MAC SEND TASK] start send package.               \n");

    uint32_t cnt = 0;
    i = 4;
    while (i > 0)
    {
        sys_net_send(test_mac.td, test_mac.td_phy);
        cnt += PNUM;
        sys_move_cursor(1, print_location);
        printf("> [MAC SEND TASK] totally send package %d !        \n", cnt);
        i--;
    }
    sys_exit();
}

void mac_recv_task()
{

    mac_t test_mac;
    uint32_t i;
    uint32_t ret;
    uint32_t print_location = 1;

    recv_num_now = 0; // step2 

    test_mac.mac_addr = 0xbfe10000;
    test_mac.dma_addr = 0xbfe11000;

    test_mac.psize = PSIZE * 4; // 64bytes
    test_mac.pnum = PNUM;       // pnum
    mac_recv_desc_init(&test_mac);

    dma_control_init(&test_mac, DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128);
    clear_interrupt(&test_mac);

    mii_dul_force(&test_mac);

    queue_init(&recv_block_queue);
    sys_move_cursor(1, print_location);
    printf("[MAC RECV TASK] start recv:                    ");
    ret = sys_net_recv(test_mac.rd, test_mac.rd_phy, test_mac.daddr);
    if (ret == 0)
    {
        sys_move_cursor(1, print_location);
        printf("[MAC RECV TASK]     net recv is ok!                          ");
    }
    else
    {
        sys_move_cursor(1, print_location);
        printf("[MAC RECV TASK]     net recv is fault!                       ");
    }

    ch_flag = 0;
    for (i = 0; i < PNUM; i++)
    {
        recv_flag[i] = 0;
    }

    //uint32_t cnt = 0;
    uint32_t *Recv_desc;
    Recv_desc = (uint32_t *)(test_mac.rd + (PNUM - 1) * 16);
    //printf("(test_mac.rd 0x%x ,Recv_desc=0x%x,REDS0 0X%x\n", test_mac.rd, Recv_desc, *(Recv_desc));
    
    
    if (((*Recv_desc) & 0x80000000) == 0x80000000)
    {
        tmp_recv = &(rx_desc_list[PNUM-1]);
        sys_move_cursor(1, print_location);
        printf("> [MAC RECV TASK] waiting receive package.\n");
        sys_wait_recv_package();
    }

    //init_screen();
    /*
    cnt += PNUM;
    sys_move_cursor(1, print_location);
    printf("> [MAC RECV TASK]print recv  buffer        \n");
    uint32_t recv_buffer, snd_buffer;
    desc_t *recv = NULL;
    recv_buffer = test_mac.daddr;
    snd_buffer = test_mac.saddr;
    print_location = 3;

    uint32_t valid_num = 0;
    for (i = 0; i < PNUM; i++)
    {
        if (recv_flag[i] == 0)
        {
            sys_move_cursor(1, print_location);
            printf("> [MAC RECV TASK]still waiting receive %dth package.             \n", i);
            sys_wait_recv_package();
        }

        Recv_desc = (uint32_t *)(test_mac.rd + i * 16);
        recv = (desc_t *)Recv_desc;

        sys_move_cursor(1, print_location);
        printf("\n%d recv buffer,r_desc( 0x%x) =0x%x:          \n", i, Recv_desc, *(Recv_desc));

        recv_buffer = recv->tdes2;
        valid_num += printf_recv_buffer((recv->tdes2 | 0xa0000000));
        sys_sleep(1);
        printf("\n");
    }

    print_location = 3;
    sys_move_cursor(1, print_location);
    printf("\nrecv valid %d packages!:\n                ", valid_num);
*/


    mac_recv_handle(&test_mac);
    sys_exit();
}

void mac_init_task()
{
    uint32_t print_location = 1;
    sys_move_cursor(1, print_location);
    printf("> [MAC INIT TASK] Waiting for MAC initialization .\n");
    sys_init_mac();
    sys_move_cursor(1, print_location);
    printf("> [MAC INIT TASK] MAC initialization succeeded.           \n");
    sys_exit();
}
