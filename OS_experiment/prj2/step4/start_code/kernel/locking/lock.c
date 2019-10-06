#include "lock.h"
#include "sched.h"
#include "syscall.h"


//extern queue_t ready_queue;

//extern pcb_t* current_running;

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

/*
int do_mutex_lock_acquire(mutex_lock_t *lock, pcb_t* pcb_pointer) // I changed the definition
{
    if(lock->status == UNLOCKED)
    {
        lock->status = LOCKED;
        //lock->locked_process_pid = pcb_pointer->pid;
        //lock->address_of_locked_process = pcb_pointer;

        //pcb_pointer->lock_address = lock;
        //pcb_pointer->status = TASK_RUNNING;

        return 1;
    }
    else if (pcb_pointer->pid == lock->locked_process_pid) // it is the process which acquired the lock
    {
       
            return 1;
    }
    else    // the lock has been acquired by other process
    {
        //queue_push(&(lock->block_queue), pcb_pointer);
        //pcb_pointer->status = TASK_BLOCKED;
        return 0;
    }
    
    
}
*/

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
    if(lock->status == UNLOCKED)
    {
        lock->status = LOCKED;
        lock->locked_process_pid = current_running->pid;
        lock->address_of_locked_process = (uint32_t)current_running;
        //current_running->lock_address = lock;
    }
    else
    {
        /* code */
        do_block(&(lock->block_queue));
    }
    
}



void do_mutex_lock_release(mutex_lock_t *lock)
{// this function just release one process out of the array.

    lock->address_of_locked_process = NULL;
    lock->locked_process_pid = 0;

    lock->status = UNLOCKED;

    do_unblock_one(&lock->block_queue);

}
