#include "cond.h"
#include "lock.h"
extern mutex_lock_t lock1;
extern mutex_lock_t lock2;
extern mutex_lock_t lock3;

void do_condition_init(condition_t *condition)
{
    queue_init(&(condition->condition_queue));
}

void do_condition_wait(mutex_lock_t *lock, condition_t *condition)
{
    do_mutex_lock_release(lock, current_running);
    //do_block(&(condition->condition_queue));

    if(current_running->target_lock_id[0] == GET_LOCK)
        do_mutex_lock_release(&lock1, current_running);
    if(current_running->target_lock_id[1] == GET_LOCK)
        do_mutex_lock_release(&lock2, current_running);
    if(current_running->target_lock_id[2] == GET_LOCK)
        do_mutex_lock_release(&lock3, current_running);

    current_running->target_lock_id[lock->lock_self_id] = NOT_GET_LOCK;
    current_running->status = TASK_BLOCKED;
    queue_push(&(condition->condition_queue), current_running);

    do_scheduler(); //need it?
    do_mutex_lock_acquire(lock);
}

void do_condition_signal(condition_t *condition)
{
    do_unblock_one(&(condition->condition_queue));
}

void do_condition_broadcast(condition_t *condition)
{
    do_unblock_all(&(condition->condition_queue));
}