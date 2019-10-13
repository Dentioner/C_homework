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

queue_t ready_queue;

/*
queue_t ready_queue_p1;
queue_t ready_queue_p2;
queue_t ready_queue_p3;
*/


//queue_t block_queue;    //it may not be used in part1
mutex_lock_t block_queue_array[NUM_MUTEX_LOCK]; // part2 mutex lock
extern mutex_lock_t mutex_lock;

queue_t sleep_queue; // part4

pid_t total_pid = 1; // this is used to init a new pcb and give it a pid

static void check_sleeping()
{
    pcb_t *tmp_pcb, *output_pcb;
    uint32_t current_time = get_timer();
    output_pcb = NULL;
    tmp_pcb = (pcb_t *)sleep_queue.head;

    while(tmp_pcb != NULL)
    {
        if(current_time >= tmp_pcb->wake_up_time)
        {
            output_pcb = tmp_pcb;
            tmp_pcb = (pcb_t *)queue_remove(&sleep_queue, output_pcb);
            //TODO: here need to give a priority
            output_pcb->status = TASK_READY;
            output_pcb->wake_up_time = 0;
            queue_push(&ready_queue, output_pcb);


		//test
//		printk("check sleep done.\n");
//		while(1);

        }
        else
        {
            tmp_pcb = tmp_pcb->next;
        }
        
    }

}


void quick_sort(pcb_t* s[], int l, int r)
{
	if (l < r)
	{
		
		int i = l, j = r;
        pcb_t* x = (pcb_t *)s[l];
		while (i < j)
		{
			while (i < j && s[j]->running_counter >= x->running_counter) 
				j--;
			if (i < j)
				s[i++] = s[j];

			while (i < j && s[i]->running_counter < x->running_counter)
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quick_sort(s, l, i - 1);
		quick_sort(s, i + 1, r);
	}
}


void queue_sort(queue_t *queue)
{
    pcb_t *temp_array[NUM_MAX_TASK];
    int index = 0;
    int i;
    //memset(temp_array, 0, NUM_MAX_TASK*sizeof(pcb_t*));
    while (!queue_is_empty(queue))
    {
        temp_array[index++] = queue_dequeue(queue);
    }
    
    quick_sort(temp_array, 0, index - 1);
    for(i = 0; i < index; i++)
    {
        queue_push(queue, temp_array[i]);
    }

    return;
}
/*
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
*/

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
    
	check_sleeping();
    if(current_running != NULL && current_running->status == TASK_RUNNING)
    {
        current_running->status = TASK_READY;
        
        //unblock();
        //if(current_running->pid != 2)
        queue_push(&ready_queue, current_running);
        queue_sort(&ready_queue);
        //ready_queue_push(current_running);
    }
    current_running = queue_dequeue(&ready_queue);
    current_running->running_counter++;
    /*
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
    */

    // because queue is FIFO, so the pcb that current_running pointed earlier
    // will not be pushed out firstly


    //test.................
    vt100_move_cursor(1, 7);
    printk("pcb[1]:%d\t", pcb[1].status);
//    vt100_move_cursor(1, 6);    
    printk("pcb[2]:%d\t", pcb[2].status);
//    vt100_move_cursor(1, 7);
    printk("pcb[3]:%d\t", pcb[3].status);
  //  vt100_move_cursor(1, 8);

    printk("pcb[4]:%d\n", pcb[4].status);
   // vt100_move_cursor(1, 9);



    vt100_move_cursor(1, 8);
    printk("pcb[5]:%d\t", pcb[5].status);
   // vt100_move_cursor(1, 10);
	printk("pcb[6]:%d\t", pcb[6].status);
	printk("pcb[7]:%d\t", pcb[7].status);
	printk("pcb[8]:%d\n", pcb[8].status);

    
    vt100_move_cursor(1, 9);
	    printk("pcb[0]:0x%x\t", &pcb[0]);
    printk("pcb[1]:0x%x\t", &pcb[1]);
   
    printk("pcb[2]:0x%x\t", &pcb[2]);

    printk("pcb[3]:0x%x\n", &pcb[3]);

vt100_move_cursor(1, 10);
    printk("pcb[4]:0x%x\t", &pcb[4]);


    printk("pcb[5]:0x%x\t", &pcb[5]);
	printk("pcb[6]:0x%x\t", &pcb[6]);
	printk("pcb[7]:0x%x\n", &pcb[7]);

 vt100_move_cursor(1, 11);
	printk("1:%u\t", pcb[1].running_counter);
	printk("2:%u\t", pcb[2].running_counter);
	printk("3:%u\n", pcb[3].running_counter);
 vt100_move_cursor(1, 12);	
	printk("4:%u\t", pcb[4].running_counter);
	printk("5:%u\t", pcb[5].running_counter);
	printk("6:%u\n", pcb[6].running_counter);
	printk("7:%u\t", pcb[7].running_counter);
	printk("8:%u\t", pcb[8].running_counter);
	printk("0:%u\n", pcb[0].running_counter);
	printk("current_running: 0x%x\n", current_running);
	printk("head:0x%x\t\t", ready_queue.head);
  	printk("tail:0x%x\n", ready_queue.tail);
  	


/* if(current_running == &pcb[1] || current_running == &pcb[1])
	{
		int asdfg;
		vt100_move_cursor(1, 9);
		printk("lock running\n");
		for(asdfg = 0; asdfg < 10000000; asdfg ++);
	}*/

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
    current_running->status = TASK_SLEEPING;
    current_running->wake_up_time = get_timer() + sleep_time;
    queue_push(&sleep_queue, current_running);
    do_scheduler();
}



void do_block(queue_t *queue)
{
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    queue_push(queue, current_running);

//	printk("test. before do_scheduler.\n");
//	while(1);

    do_scheduler();
	

    do_mutex_lock_acquire(&mutex_lock);
}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
    pcb_t * pointer = NULL;


    if(!queue_is_empty(queue))
    {    
        pointer = (pcb_t *)queue_dequeue(queue);
        pointer->status = TASK_READY;
        queue_push(&ready_queue, pointer);
    }
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
    while(!queue_is_empty(queue))
    {
        do_unblock_one(queue);
    }
}

