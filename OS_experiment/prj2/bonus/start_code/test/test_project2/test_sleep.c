#include "test2.h"
#include "sched.h"
#include "stdio.h"
#include "syscall.h"

static char blank[] = {"                                                "};

void sleep_task(void)
{
    int i;
    int print_location = 6;
    int sleep_time = 5;

    while (1)
    {
        for (i = 0; i < 20; i++)
        {
            sys_move_cursor(1, print_location);
            printf("> [TASK] This task is to test sleep(). (%d)\n", i);
        }

	//printk("test : the front of sleeping.\n");
	//while(1);

        sys_move_cursor(1, print_location);
        printf("> [TASK] This task is sleeping, sleep time is %d.\n", sleep_time);

        /* call syscall sleep() */
        sys_sleep(sleep_time);

        sys_move_cursor(1, print_location);
        printf("%s", blank);
	


	
	/*while(1)
	{	
		vt100_move_cursor(1, 5);
		printk("cp0 = %d, status = %d\n", (int)get_count(), (int)get_compare());
	
	}*/
//	printk("test. end of the sleep task.\n");
//	while(1);
    
    }
}
