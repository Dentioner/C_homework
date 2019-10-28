#include "barrier.h"
#include "lock.h"

extern mutex_lock_t lock1;
extern mutex_lock_t lock2;
extern mutex_lock_t lock3;

void do_barrier_init(barrier_t *barrier, int goal)
{
    queue_init(&(barrier->barrier_queue));
    barrier->goal = goal;
    barrier->arrived_num = 0;
}

void do_barrier_wait(barrier_t *barrier)
{
    barrier->arrived_num++;
    if(barrier->arrived_num >= barrier->goal)
    {
        do_unblock_all(&(barrier->barrier_queue));
        barrier->arrived_num = 0;
    }
    else
    {
        if(current_running->target_lock_id[0] == GET_LOCK)
            do_mutex_lock_release(&lock1, current_running);
        if(current_running->target_lock_id[1] == GET_LOCK)
            do_mutex_lock_release(&lock2, current_running);
        if(current_running->target_lock_id[2] == GET_LOCK)
            do_mutex_lock_release(&lock3, current_running);

    
        current_running->status = TASK_BLOCKED;
        queue_push(&(barrier->barrier_queue), current_running);

        do_scheduler(); //need it?
    }
    
}