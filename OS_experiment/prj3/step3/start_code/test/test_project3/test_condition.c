#include "lock.h"
#include "cond.h"
#include "time.h"
#include "stdio.h"
#include "test3.h"
#include "syscall.h"

//static mutex_lock_t mutex;
mutex_lock_t lock3;
static condition_t condition;
static int num_staff = 0; // it is the resource that consumers can use currently

void producer_task(void)
{
    int i;
    int print_location = 0;
    int production = 3;
    int sum_production = 0;
    pcb_t *tmp;

    for (i = 0; i < 50; i++)
    {
        //mutex_lock_acquire(&mutex);
        mutex_lock_acquire(&lock3);
        num_staff += production;
        sum_production += production;

        //mutex_lock_release(&mutex, current_running);
        mutex_lock_release(&lock3, current_running);

        sys_move_cursor(0, print_location);
        printf("> [TASK] Total produced %d products.      ", sum_production);

        condition_signal(&condition);
        //condition_broadcast(&condition);

        sys_sleep(1);
    }

    while(!queue_is_empty(&(condition.condition_queue)))
    {
        tmp = queue_dequeue(&(condition.condition_queue));
        tmp->status = TASK_EXITED;
    }
    sys_exit();
}

void consumer_task1(void)
{
    int print_location = 1;
    int consumption = 1;
    int sum_consumption = 0;

    while (1)
    {
        //mutex_lock_acquire(&mutex);
        mutex_lock_acquire(&lock3);

        while (num_staff == 0)
        {
        //    condition_wait(&mutex, &condition);
            condition_wait(&lock3, &condition);
        
        }

        num_staff -= consumption;
        sum_consumption += consumption;

        sys_move_cursor(0, print_location);
        printf("> [TASK] Total consumed %d products.", sum_consumption);

        //mutex_lock_release(&mutex, current_running);
        mutex_lock_release(&lock3, current_running);
    
    }
}

void consumer_task2(void)
{
    int print_location = 2;
    int consumption = 1;
    int sum_consumption = 0;

    while (1)
    {
        //mutex_lock_acquire(&mutex);
        mutex_lock_acquire(&lock3);

        while (num_staff == 0)
        {
            //condition_wait(&mutex, &condition);
            condition_wait(&lock3, &condition);
        
        }

        num_staff -= consumption;
        sum_consumption += consumption;

        sys_move_cursor(0, print_location);
        printf("> [TASK] Total consumed %d products.", sum_consumption);

//        mutex_lock_release(&mutex, current_running);
        mutex_lock_release(&lock3, current_running);

    }
}