#include "sem.h"
#include "stdio.h"
#include "sched.h"

void do_semaphore_init(semaphore_t *s, int val)
{
    if (val < 0)
    {
        printk("invalid value for semaphore!\n");
        while(1);
    }
    s->value = val;
    queue_init(&(s->semaphore_queue));
}

// semaphore_up method 1
void do_semaphore_up(semaphore_t *s) // V(sv)
{
    s->value++;
    if(s->value <= 0)
    {
        // which one is better?
        // do_unblock_all(&(s->semaphore_queue));
        do_unblock_one(&(s->semaphore_queue));
    }

    // do_scheduler();
}


// semaphore_up method 2
/*
void do_semaphore_up(semaphore_t *s) // V(sv)
{
    if(queue_is_empty(&(s->semaphore_queue)))
    {
        s->value++;
    }
    else
    {
        // which one is better?
        // do_unblock_all(&(s->semaphore_queue));
        do_unblock_one(&(s->semaphore_queue)); 
    }
    // do_scheduler();
}
*/



void do_semaphore_down(semaphore_t *s) // P(sv)
{
    s->value--;
    if(s->value < 0)
    {
        current_running->status = TASK_BLOCKED;
        queue_push(&(s->semaphore_queue), current_running);
    }

    // do_scheduler();
}