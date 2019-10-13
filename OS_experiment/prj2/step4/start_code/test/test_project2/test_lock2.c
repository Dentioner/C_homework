#include "test2.h"
#include "lock.h"
#include "stdio.h"
#include "syscall.h"

int is_init = FALSE;
static char blank[] = {"                                             "};

/* if you want to use spin lock, you need define SPIN_LOCK */
//  #define SPIN_LOCK
spin_lock_t spin_lock;

/* if you want to use mutex lock, you need define MUTEX_LOCK */
#define MUTEX_LOCK
mutex_lock_t mutex_lock;

void lock_task1(void)
{
        int print_location = 3;
//        int print_location = 1;
	int print_location_test = 1;
        while (1)
        {
                int i;

		//printk("test: before IF. init = %d\n", is_init);
		//while(1);

                if (!is_init)
                {

#ifdef SPIN_LOCK
                        spin_lock_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
			//printk("test here. before all the syscall\n");
			//while(1);
			
				
                        mutex_lock_init(&mutex_lock);
			//printk("test:lock init finished.\n");
			//while(1);

#endif
                        is_init = TRUE;
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Applying for a lock.\n");


		//printk("test: 2 move cursor finished.\n");
		//while(1);
		
#ifdef SPIN_LOCK
                spin_lock_acquire(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mutex_lock_acquire(&mutex_lock);
		//printk("test: lock acquire finished.\n");
		//while(1);	
		
#endif

		
                for (i = 0; i < 20; i++)
                {
                        sys_move_cursor(1, print_location);
                      //vt100_move_cursor(1, print_location);
			printf("> [TASK] Has acquired lock and running.(%d)\n", i);
			//if (i == 2)
			//{	
				//vt100_move_cursor(1, print_location);
				//printk("%s", blank);
				//vt100_move_cursor(1, 5);
				//if(i < 10)
				//	printk("test. i =  %d\n", i);
				//else
				//	printk("test. i = %d\n", i);			
				//while(1);
			//}
			//printk("\n");
		//	vt100_move_cursor(1, print_location);
			//printk("> [TASK] Has acquired lock and running.(%d)\n", i);
			
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Has acquired lock and exited.\n");

		//printk("test: 4 move cursor finished.\n");
		//while(1);
		

#ifdef SPIN_LOCK
                spin_lock_release(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mutex_lock_release(&mutex_lock);
		//printk("test: release finished.\n");
		//while(1);
		
#endif
        }
}

void lock_task2(void)
{
        int print_location = 4;
        while (1)
        {
                int i;
	
		//printk("debug: checkpoint 0.init = %d\n", is_init);
		//while(1);	

                if (!is_init)
                {

#ifdef SPIN_LOCK
                        spin_lock_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                        mutex_lock_init(&mutex_lock);
			//printk("debug: checkpoint 1.\n");
			//while(1);
#endif
                        is_init = TRUE;
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Applying for a lock.\n");
			//printk("debug: checkpoint 2.\n");
			//while(1);

#ifdef SPIN_LOCK
                spin_lock_acquire(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mutex_lock_acquire(&mutex_lock);
			//printk("debug: checkpoint 3.\n");
			//while(1);
#endif

                for (i = 0; i < 20; i++)
                {
                        sys_move_cursor(1, print_location);
                        printf("> [TASK] Has acquired lock and running.(%d)\n", i);
                }
			//printk("debug: checkpoint 4.\n");
			//while(1);

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Has acquired lock and exited.\n");
			//printk("debug: checkpoint 5.\n");
			//while(1);

#ifdef SPIN_LOCK
                spin_lock_release(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mutex_lock_release(&mutex_lock);
			//printk("debug: checkpoint 6.\n");
			//while(1);
#endif
        }
}
