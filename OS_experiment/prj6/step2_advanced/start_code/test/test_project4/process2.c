#include "sched.h"
#include "stdio.h"
#include "syscall.h"
#include "time.h"
#include "screen.h"
#include "test4.h"

#define RW_TIMES 3

int rand()
{	
	int current_time = get_timer();
	return current_time % 100000;
}

/*static void disable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status &= 0xfffffffe;
    set_cp0_status(cp0_status);
}

static void enable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status |= 0x01;
    set_cp0_status(cp0_status);
}

static char read_uart_ch(void)
{
    char ch = 0;
    unsigned char *read_port = (unsigned char *)(0xbfe48000 + 0x00);
    unsigned char *stat_port = (unsigned char *)(0xbfe48000 + 0x05);

    while ((*stat_port & 0x01))
    {
        ch = *read_port;
    }
    return ch;
}
*/

void rw_task1(unsigned long a[6])
{
	int index1;
	//test...
	/*for(index1 = 0; index1 < 6; index1 ++)
	{
		printf("a[%d] = %u\t", index1, (uint32_t)a[index1]);
	}
	printf("\n");
	while(1);*/
	sys_move_cursor(1, 1);
	printf("                                                    ");

	/*disable_all_interrupt();
	vt100_move_cursor(1, 1);
	printk("                                                  ");
	open_all_interrupt();*/
	
	int mem1, mem2 = 0;
	int curs = 0;
	int memory[RW_TIMES];
	int i = 0;
	for(i = 0; i < RW_TIMES; i++)
	{
		//vt100_move_cursor(1, curs+i);
		sys_move_cursor(1, curs+i);
		mem1 = a[i];
		//vt100_move_cursor(1, curs+i);
		sys_move_cursor(1, curs+i);
		memory[i] = mem2 = rand();
		*(int *)mem1 = mem2;
		printf("                                                            ");
		sys_move_cursor(1, curs+i);
		printf("Write: 0x%x, %d                                             ", mem1, mem2);
	}
	curs = RW_TIMES;
	for(i = 0; i < RW_TIMES; i++)
	{
		//vt100_move_cursor(1, curs+i);
		sys_move_cursor(1, curs+i);
		mem1 = a[RW_TIMES+i];
		//vt100_move_cursor(1, curs+i);
		sys_move_cursor(1, curs+i);
		memory[i+RW_TIMES] = *(int *)mem1;
		if(memory[i+RW_TIMES] == memory[i])
			printf("Read succeed: %d                                             ", memory[i+RW_TIMES]);
		else
			printf("Read error: %d                                             ", memory[i+RW_TIMES]);
	}
	while(1);
	//Input address from argv.
}
