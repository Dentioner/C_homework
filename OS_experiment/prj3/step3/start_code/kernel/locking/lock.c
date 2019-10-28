#include "lock.h"
#include "sched.h"
#include "syscall.h"


//extern queue_t ready_queue;

//extern pcb_t* current_running;
extern mutex_lock_t lock1;
extern mutex_lock_t lock2;
extern mutex_lock_t lock3;
void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void spin_lock_acquire(spin_lock_t *lock)
{
    while (LOCKED == lock->status)
    {
    };
    lock->status = LOCKED;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
    lock->status = UNLOCKED;
    queue_init(&(lock->block_queue));
    lock->locked_process_pid = 0;
    lock->address_of_locked_process = NULL;
}


void do_mutex_lock_acquire(mutex_lock_t *lock)
{

    current_running->target_lock_id[lock->lock_self_id] = WANT_LOCK;
    if(lock->status == UNLOCKED)
    {
        lock->status = LOCKED;
        lock->locked_process_pid = current_running->pid;
        lock->address_of_locked_process = (uint32_t)current_running;
        //current_running->lock_address = lock;
        current_running->target_lock_id[lock->lock_self_id] = GET_LOCK;
    }
    else
    {
        /* code */
        int index1;
        if(current_running->target_lock_id[0] == GET_LOCK)
            do_mutex_lock_release(&lock1, current_running);
        if(current_running->target_lock_id[1] == GET_LOCK)
            do_mutex_lock_release(&lock2, current_running);
        if(current_running->target_lock_id[2] == GET_LOCK)
            do_mutex_lock_release(&lock3, current_running);

        current_running->target_lock_id[lock->lock_self_id] = NOT_GET_LOCK;
        do_block(&(lock->block_queue));
    }
    
}



void do_mutex_lock_release(mutex_lock_t *lock, pcb_t * process)
{// this function just release one process out of the array.

    //pcb_t *tmp = &pcb[index_of_pcb];
    lock->address_of_locked_process = NULL;
    lock->locked_process_pid = 0;

    lock->status = UNLOCKED;

    do_unblock_one(&lock->block_queue);

    if(process->target_lock_id[lock->lock_self_id] == GET_LOCK)
        process->target_lock_id[lock->lock_self_id] = WANT_LOCK;
    else
    {
        printk("error condition of lock.\n");
        while(1);
    }
    
}

