#ifndef INCLUDE_MAC_H_
#define INCLUDE_MAC_H_

#include "type.h"
#include "queue.h"
#include "sched.h"
#include "syscall.h"

//#define TEST_REGS1
//#define TEST_REGS2
#define TEST_REGS3
#define GMAC_BASE_ADDR (0xbfe10000)
#define DMA_BASE_ADDR (0xbfe11000)
#define PSIZE (256) // every package size is 256 Byte
#define PNUM (64)   // 64 packages

//#define TX_DESC_NUM 64
//#define RX_DESC_NUM 64

#define RX_HAS_LINK 0x01000000
#define RX_LINK_END 0x02000000

#define TX_HAS_LINK 0x01000000
#define TX_LINK_END 0x02000000

#define DESC_OWN    0x80000000

#define RECV_ERROR 0x4000f8cf

#define BUFFER_SIZE 0x400 // 4*256 = 1024 = 0x400,   4 = sizeof(uint32_t),  256 = length(buffer[])


#define TX_LS 0x40000000
#define TX_FS 0x20000000
extern queue_t recv_block_queue;
extern uint32_t recv_flag[PNUM];
extern uint32_t ch_flag;
enum GmacRegisters
{
    GmacAddr0Low = 0x0044,  /* Mac frame filtering controls */
    GmacAddr0High = 0x0040, /* Mac address0 high Register  */
};
enum DmaRegisters
{
    DmaTPD = 0x0004,       /* CSR1 - Transmit Poll Demand Register              */
    DmaRPD = 0x0008,       /* CSR2 - Receive Poll Demand Register               */
    DmaStatus = 0x0014,    /* CSR5 - Dma status Register                        */
    DmaInterrupt = 0x001C, /* CSR7 - Interrupt enable                           */
    DmaControl = 0x0018,   /* CSR6 - Dma Operation Mode Register                */
};
enum DmaControlReg
{

    DmaStoreAndForward = 0x00200000, /* (SF)Store and forward                            21      RW        0       */
    DmaRxThreshCtrl128 = 0x00000018, /* (RTC)Controls thre Threh of MTL tx Fifo 128      4:3   RW                */

    DmaTxStart = 0x00002000, /* (ST)Start/Stop transmission                      13      RW        0       */

    DmaTxSecondFrame = 0x00000004, /* (OSF)Operate on second frame                     4       RW        0       */
};
enum InitialRegisters
{
    DmaIntDisable = 0,
};
typedef struct desc
{
    uint32_t tdes0; //  |   own         |status[30:0]|
    uint32_t tdes1; //  |Ctrl Bits[9:0] |Byte count buffer2[10:0]|Byte count buffer1[10:0]|
    uint32_t tdes2; //  buffer1 addr
    uint32_t tdes3; //  buffer2 addr or Next Desc addr
} desc_t;
typedef struct mac
{
    uint32_t psize; // backpack size
    uint32_t pnum;
    uint32_t mac_addr; // MAC base address
    uint32_t dma_addr; // DMA base address

    uint32_t saddr; // send address
    uint32_t daddr; // receive address

    uint32_t saddr_phy; // send phy address
    uint32_t daddr_phy; // receive phy address

    uint32_t td; // DMA send desc
    uint32_t rd; // DMA receive desc

    uint32_t td_phy;
    uint32_t rd_phy;

} mac_t;

uint32_t read_register(uint32_t base, uint32_t offset);
void reg_write_32(uint32_t addr, uint32_t data);

void print_rx_dscrb(mac_t *mac);
void print_tx_dscrb(mac_t *mac);
uint32_t do_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr);
void do_net_send(uint32_t td, uint32_t td_phy);
void do_init_mac(void);
void do_wait_recv_package(void);
void mac_irq_handle(void);
void mac_recv_handle(mac_t *test_mac);


desc_t tx_desc_list[PNUM];
desc_t rx_desc_list[PNUM];

//char send_package[PNUM][PSIZE];
//char recv_package[PNUM][PSIZE];

uint32_t send_package[PNUM][PSIZE];
uint32_t recv_package[PNUM][PSIZE];

#endif

