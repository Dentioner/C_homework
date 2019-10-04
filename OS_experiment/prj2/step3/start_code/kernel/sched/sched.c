#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"
#include "lock.h"	// part2


pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;
//unsigned int pcb_array_length = 0;


/* global process id */
pid_t process_id = 1;

//there are 2 queues in sched.h headed by "extern"
//so do I need to define them here?

// queue_t ready_queue;
queue_t ready_queue_p1;
queue_t ready_queue_p2;
queue_t ready_queue_p3;



//queue_t block_queue;    //it may not be used in part1
mutex_lock_t block_queue_array[NUM_MUTEX_LOCK]; // part2 mutex lock
extern mutex_lock_t mutex_lock;

pid_t total_pid = 1; // this is used to init a new pcb and give it a pid

static void check_sleeping()
{
}

void ready_queue_push(pcb_t * pcb)
{
    if(pcb->priority == P1)
    {
        pcb->priority = P2;
        queue_push(&ready_queue_p2, pcb);
    }
    else if(pcb->priority == P2)
    {
        pcb->priority = P3;
        queue_push(&ready_queue_p3, pcb);
    }
    else
    {
        queue_push(&ready_queue_p3, pcb);
    }
    
}

void switch_pcb()
{
    pcb_t * temp_process_pointer = current_running;
    int i;
    int not_be_blocked = 1;
    /*if(current_running != NULL)
    {
        queue_push(&ready_queue, current_running);
    }
    current_running = queue_dequeue(&ready_queue);
    */
    // code above is part1
    // the following is part2
    
    if(current_running != NULL && current_running->status == TASK_RUNNING)
    {
        current_running->status = TASK_READY;
        
        //unblock();
        //if(current_running->pid != 2)
        //queue_push(&ready_queue, current_running);
        ready_queue_push(current_running);
    }
    //current_running = queue_dequeue(&ready_queue);

    if(!queue_is_empty(&ready_queue_p1))
    {
        current_running = queue_dequeue(&ready_queue_p1);
    }
    else if(!queue_is_empty(&ready_queue_p2))
    {
        current_running = queue_dequeue(&ready_queue_p2);
    }
    else if(!queue_is_empty(&ready_queue_p3))
    {
        current_running = queue_dequeue(&ready_queue_p3);
    }
    else
    {
        printk("No process. Error condition.\n");
        while(1);
    }
    

    // because queue is FIFO, so the pcb that current_running pointed earlier
    // will not be pushed out firstly


    return;
}

void scheduler(void)
{
    // TODO schedule
    // Modify the current_running pointer.
    // scheduler() works accorrding to the definition style of pcb structure 
    // code with "//" is pseudo code
    // printk("Debugging: I am in the front of schduler().\n");

    // because I cannot get screen_cursor in entry.S, I save these two variables here :-(
    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;


    //switch

    //printk("before : lock: status: %d, pid: %d\n", mutex_lock.status, mutex_lock.locked_process_pid);


    switch_pcb();
   // printk("Debugging: I am in the rear of switch_pcb().\n");



    if(current_running->first_running)// init a pcb that is waken first time
    {
        current_running->pid = total_pid;
        total_pid++;
        current_running->first_running = 0;
    }

    /* register context */
    //it will be done in entry.S
    /* previous, next pointer */

    /* process id */
    process_id = current_running->pid;
    /* kernel/user thread/process */
    
    /* BLOCK | READY | RUNNING */
    current_running->status = TASK_RUNNING;
    /* cursor position */

  
    

    //because I cannot get screen_cursor in entry.S, I restore these two variables here :-(
    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;

   // printk("Debugging: I am in the last line of scheduler().\n");
    
    //printk("after : lock: status: %d, pid: %d\n", mutex_lock.status, mutex_lock.locked_process_pid);
}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
}



void do_block(queue_t *queue)
{
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    queue_push(queue, current_running);
    do_scheduler();
    do_mutex_lock_acquire(&mutex_lock);
}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
    pcb_t * pointer = NULL;

    pointer = (pcb_t *)queue_dequeue(queue);

    pointer->status = TASK_READY;
    queue_push(&ready_queue, pointer);

}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
    while(!queue_is_empty(queue))
    {
        do_unblock_one(queue);
    }
}

