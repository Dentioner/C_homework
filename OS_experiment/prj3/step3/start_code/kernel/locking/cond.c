#include "cond.h"
#include "lock.h"

void do_condition_init(condition_t *condition)
{
    queue_init(&(condition->condition_queue));
}

void do_condition_wait(mutex_lock_t *lock, condition_t *condition)
{
    
}

void do_condition_signal(condition_t *condition)
{
    do_unblock_one(&(condition->condition_queue));
}

void do_condition_broadcast(condition_t *condition)
{
    do_unblock_all(&(condition->condition_queue));
}