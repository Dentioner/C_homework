#ifndef INCLUDE_MM_H_
#define INCLUDE_MM_H_

#include "type.h"

#define TLB_ENTRY_NUMBER 64
#define PGTABLE_NUM   0x400
#define PGTABLE_NUM_STACK 0x80
#define PGTABLE_NUM_ALL PGTABLE_NUM+PGTABLE_NUM_STACK    
#define PAGEFRAME_NUM 0x400  // 暂定使用8MB的内存大小...

#define PGFRAME_SIZE 0x1000 // 4KB

/*
ctrl:

|Cacheable|Reference|Modified|Protection|Valid| ? |
|   1     |    1    |   1    |     3    |  1  | 1 |

total: 1+1+1+3+1=7 bit, 1bit idle
init value:
cacheable = 0; reference = 0; modified = 0; protection = 111; valid = 0;

M point is used as "Dirty"
R point is used for page fault, see textbook P109 
C point is used in Cache mechanism
V point is used for page fault, not for TLB invalid !!!!

*/

#define PTE_V       0x2       // binary: 0000 0010
#define PTE_P_R     0x4       // binary: 0000 0100  => read  protection
#define PTE_P_W     0x8       // binary: 0000 1000  => write protection
#define PTE_P_E     0x10      // binary: 0001 0000  => execute protection
#define PTE_M       0x20      // binary: 0010 0000
#define PTE_R       0x40      // binary: 0100 0000
#define PTE_C       0x80      // binary: 1000 0000

#define PF_BASE     0x1600000 // 暂定的页框覆盖的基址，从此地址往后为页框的作用范围
#define PFN_BASE    PF_BASE>>12

typedef struct pte
{
    uint32_t vpn;
    uint32_t pfn;
    uint8_t ctrl; // controller bits
    pid_t pte_pid;
}pte_t;

typedef struct page_frame_controller
{
    //uint32_t pfn;
    pid_t pfn_pid;
    uint8_t used;    
}pf_t;

/* TLB exception handler entery */
extern void TLBexception_handler_entry(void);
extern void TLBexception_handler_begin(void);
extern void TLBexception_handler_end(void);


void do_TLB_Refill();
void do_page_fault();


pte_t page_table[PGTABLE_NUM_ALL];

pf_t page_frame_table[PAGEFRAME_NUM];


#endif
