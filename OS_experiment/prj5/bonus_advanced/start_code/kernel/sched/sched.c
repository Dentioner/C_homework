#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"
#include "lock.h"	// part2
#include "irq.h"    // part3
//#include "mm.h"   // part4

pcb_t pcb[NUM_MAX_TASK];
extern mutex_lock_t lock_array[NUM_MAX_LOCK];
/* current running task PCB */
pcb_t *current_running;
//unsigned int pcb_array_length = 0;


/* global process id */
pid_t process_id = 1;
uint32_t lock_num = 0;

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
// extern mutex_lock_t mutex_lock;


extern uint32_t current_line;
extern uint32_t current_start_point;

queue_t sleep_queue; // part4

pid_t total_pid = 2; // this is used to init a new pcb and give it a pid

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
        if(temp_array[i]->status == TASK_READY || temp_array[i]->status == TASK_RUNNING)
            queue_push(queue, temp_array[i]);
    }
    return;
}



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

            //output_pcb->block_in_queue = NULL; //test

            queue_push(&ready_queue, output_pcb);
            queue_sort(&ready_queue); //test


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

    // this IF is used for prj5 bonus
    if((current_running->pid != 0) && (current_running->pid!= 1))
    {
        current_running->running_counter = 1;
    }

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
    vt100_move_cursor(1, 9);
    printk("pcb[0]:0x%x\t", &pcb[0]);
    printk("pcb[1]:0x%x\t", &pcb[1]);

    printk("pcb[2]:0x%x\n", &pcb[2]);
    vt100_move_cursor(1, 10);
    printk("pcb[3]:0x%x\t", &pcb[3]);
    printk("pcb[4]:0x%x\t", &pcb[4]);

    printk("pcb[5]:0x%x\n", &pcb[5]);
    vt100_move_cursor(1, 11);
    printk("pcb[6]:0x%x\t", &pcb[6]);
    printk("pcb[7]:0x%x\t", &pcb[7]);

    printk("pcb[8]:0x%x\n", &pcb[8]);


vt100_move_cursor(1, 12);
	printk("current_running: 0x%x\n", current_running);
vt100_move_cursor(1, 13);
	printk("0:%u\t", pcb[0].running_counter);
	printk("1:%u\t", pcb[1].running_counter);
	printk("2:%u\t", pcb[2].running_counter);
	printk("3:%u\t", pcb[3].running_counter);
	printk("4:%u\t", pcb[4].running_counter);
	printk("5:%u\t", pcb[5].running_counter);
	printk("6:%u\t", pcb[6].running_counter);
	printk("7:%u\t", pcb[7].running_counter);
	printk("8:%u\t", pcb[8].running_counter);
	printk("9:%u\t", pcb[9].running_counter);
vt100_move_cursor(1, 14);
	printk("0:%d\t", pcb[0].status);
	printk("1:%d\t", pcb[1].status);
	printk("2:%d\t", pcb[2].status);
	printk("3:%d\t", pcb[3].status);
	printk("4:%d\t", pcb[4].status);
	printk("5:%d\t", pcb[5].status);
	printk("6:%d\t", pcb[6].status);
	printk("7:%d\t", pcb[7].status);
	printk("8:%d\t", pcb[8].status);
	printk("9:%d\n", pcb[9].status);
*/


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

    //uint32_t tmp_entryhi;

    // because I cannot get screen_cursor in entry.S, I save these two variables here :-(
    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;


    //switch

    //printk("before : lock: status: %d, pid: %d\n", mutex_lock.status, mutex_lock.locked_process_pid);


    switch_pcb();
   // printk("Debugging: I am in the rear of switch_pcb().\n");



    /*if(current_running->first_running)// init a pcb that is waken first time
    {
        current_running->pid = total_pid;
        total_pid++;
        current_running->first_running = 0;
    }*/

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

/*
    tmp_entryhi = get_cp0_entryhi();
    tmp_entryhi = tmp_entryhi & 0xffffe000;
    tmp_entryhi = tmp_entryhi | (current_running->pid);
    set_cp0_entryhi(tmp_entryhi);
*/
    //set_cp0_entryhi(current_running->pid);

}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
    current_running->status = TASK_SLEEPING;
    current_running->wake_up_time = get_timer() + sleep_time;
    //current_running->block_in_queue = &sleep_queue;
    queue_push(&sleep_queue, current_running);
    do_scheduler();
}



void do_block(queue_t *queue)
{
    int index1;
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    current_running->block_in_queue = queue;
    queue_push(queue, current_running);

//	printk("test. before do_scheduler.\n");
//	while(1);

    do_scheduler();
	/*
    if(current_running->target_lock_id[0] == NOT_GET_LOCK)
    {
        current_running->target_lock_id[0] == WANT_LOCK;
        do_mutex_lock_acquire(&lock1);
    }
    else if(current_running->target_lock_id[1] == NOT_GET_LOCK)
    {
        current_running->target_lock_id[1] == WANT_LOCK;
        do_mutex_lock_acquire(&lock2);
    }
    else if(current_running->target_lock_id[2] == NOT_GET_LOCK)
    {
        current_running->target_lock_id[2] == WANT_LOCK;
        do_mutex_lock_acquire(&lock3);
    }*/

    for(index1 = 0; index1 < NUM_MAX_LOCK; index1++)
    {
        if(current_running->target_lock_id[index1] == NOT_GET_LOCK)
        {
            current_running->target_lock_id[index1] == WANT_LOCK;
            do_mutex_lock_acquire(&lock_array[index1]);
            break;
        }
    }
//    do_mutex_lock_acquire(&mutex_lock);

}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
    pcb_t * pointer = NULL;


    if(!queue_is_empty(queue))
    {    
        pointer = (pcb_t *)queue_dequeue(queue);
        pointer->status = TASK_READY;
        pointer->block_in_queue = NULL; //test
        queue_push(&ready_queue, pointer);
        queue_sort(&ready_queue); //test
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


void do_process_show()
{
    int i;
    int print_counter1 = 0;
    //current_line++;                         // add on user state
    vt100_move_cursor(1, current_line + 2);     // add on user state
    printk("[PROCESS TABLE]\n");
    for(i = 0; i < NUM_MAX_TASK ; i++)
    {
        //if (pcb[i].status == TASK_UNUSED)
            //continue;
            //break;
        //printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid); 
        if(pcb[i].status == TASK_RUNNING)
        {
            printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid); 
            printk("RUNNING\n");
            print_counter1++;
        }
        else if(pcb[i].status == TASK_BLOCKED)
        {
            printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid);         
            printk("BLOCKED\n");
            print_counter1++;
        }
        else if(pcb[i].status == TASK_READY)
        {
            printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid); 
            printk("READY\n");
            print_counter1++;
        }
        /*else if(pcb[i].status == TASK_EXITED)
        {
            printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid); 
            printk("EXITED\n");
            print_counter1++;
        }*/
        else if(pcb[i].status == TASK_SLEEPING)
        {
            printk("[%d]\tPID : %d\tSTATUS : ", i, pcb[i].pid); 
            printk("SLEEPING\n");
            print_counter1++;
        }
        /*else
        {
            printk("ERROR branch.\n");
            while(1);
        }*/
        
        //should I move cursor manually? Yes
        
    }
    current_line += print_counter1 + 1;
}

void do_spawn(task_info_t *task, unsigned long * argv)
{
    int index1;
    for(index1  = 0; index1 < NUM_MAX_TASK; index1++)
    {
        if(pcb[index1].status == TASK_UNUSED || pcb[index1].status == TASK_EXITED)
            break;
    }
	
    if(index1 == NUM_MAX_TASK)
    {
        printk("ERROR: not enough empty pcb.\n");
        return;
    }


//memset(pcb[index1], 0 , sizeof(pcb_t));
	
    pcb[index1].kernel_context.regs[29] = STACK_TOP - 2*index1 * STACK_SIZE;	//sp
    
    
    //pcb[i].kernel_context.regs[31] = lock_tasks[j]->entry_point;			//ra
    pcb[index1].kernel_context.regs[31] = (uint32_t)handle_int + 0x14;			//ra
    

    pcb[index1].kernel_stack_top = STACK_TOP - 2*index1 * STACK_SIZE;
    

    pcb[index1].type = task->type;
    pcb[index1].status = TASK_READY;
    pcb[index1].kernel_context.cp0_status = CP0_STATUS_INIT;
//		pcb[i].kernel_context.cp0_epc = lock_tasks[j]->entry_point;


//	pcb[i].user_context.regs[29] = pcb[i].kernel_context.regs[29];
//	pcb[i].user_context.regs[31] = pcb[i].kernel_context.regs[31];
//	pcb[i].user_stack_top = pcb[i].kernel_stack_top;
    
    
    
    pcb[index1].user_context.regs[29] = pcb[index1].kernel_stack_top - STACK_SIZE;
    //pcb[index1].user_context.regs[29] = STACK_TOP_USER - index1 * STACK_SIZE_U - 0x4;
    
    pcb[index1].user_context.regs[31] = task->entry_point;
    
    pcb[index1].user_stack_top = pcb[index1].kernel_stack_top - STACK_SIZE;
    //pcb[index1].user_stack_top = pcb[index1].user_context.regs[29];
    
    
    pcb[index1].user_context.cp0_status = CP0_STATUS_INIT;
    pcb[index1].user_context.cp0_epc = task->entry_point; // because "eret" is after "RESTORE_CONTEXT(USER)"


    //pcb[index1].running_counter = pcb[0].running_counter;
    pcb[index1].running_counter = 0; // used for prj5 bonus
    
    
    
    pcb[index1].pid = total_pid;
    
    pcb[index1].block_in_queue = NULL;
    //queue_push(&ready_queue_p1, &pcb[i]);
    queue_push(&ready_queue, &pcb[index1]);
    total_pid++;

    // prj4 added: let a0 keep the args of new process
    pcb[index1].user_context.regs[4] = (uint32_t)argv; 
    

}


int search_queue(queue_t *queue, pcb_t * process)
{
    pcb_t * head = queue->head;
    while(head != NULL)
    {
        if(head == process)
        {
            return 1;
        }
        head = head->next;
    }
    return 0;
}


void do_kill(pid_t pid)
{
    //uint32_t tmp_entryhi, tmp_entrylo0, tmp_entrylo1, tmp_asid;
    int index1, index2, index3, index4, index5;
    pcb_t * tmp_pointer = NULL;
    
        
    for(index1 = 0; index1 < NUM_MAX_TASK; index1++)
    {
        if(pcb[index1].pid == pid)
        {
            tmp_pointer = &pcb[index1];
            break;
        }
    }

    if(tmp_pointer == NULL)
    {
        vt100_move_cursor(1, current_line + 2);
        printk("In do_kill(). ERROR: PID not exists.\n");
        current_line++;
        return;
        //while(1);
    }

    for(index2 = 0; index2 < NUM_MAX_LOCK; index2++)
    {
        if(tmp_pointer->target_lock_id[index2] == GET_LOCK)
            do_mutex_lock_release(&lock_array[index2], tmp_pointer);
    }

    // release wait_queue
    do_unblock_all(&(tmp_pointer->wait_queue));

    // change status
    //tmp_pointer->status = TASK_EXITED;

    // pop from the ready_queue
    if(tmp_pointer->status == TASK_READY || tmp_pointer->status == TASK_RUNNING)
    {
        queue_remove(&ready_queue, tmp_pointer);
    }
    else if(tmp_pointer->status == TASK_BLOCKED)
    {   

        if(tmp_pointer->block_in_queue != NULL)
        {
            queue_remove(tmp_pointer->block_in_queue, tmp_pointer);
        }

    }
    else if(tmp_pointer->status == TASK_SLEEPING)
    {
        queue_remove(&sleep_queue, tmp_pointer);
    }
    // release stack space
    tmp_pointer->kernel_context.regs[29] = STACK_TOP - 2 *index1 * STACK_SIZE;
    tmp_pointer->kernel_stack_top = STACK_TOP - 2 *index1 * STACK_SIZE;
    tmp_pointer->user_context.regs[29] = pcb[index1].kernel_stack_top - STACK_SIZE;
    tmp_pointer->user_stack_top = pcb[index1].kernel_stack_top - STACK_SIZE;

    //change status
    tmp_pointer->status = TASK_EXITED;

/*
    //refresh page table
    for(index3 = 0; index3 < PGTABLE_NUM; index3++)
    {
        if(page_table[index3].pte_pid == pid)
        {
            (page_table[index3].ctrl) &= (~PTE_V); // valid = 0;
            (page_table[index3].ctrl) &= (~PTE_R); // reference = 0;
            
            page_table[index3].pte_pid = 0;

        }
    }

    // refresh page frame
    for(index4 = 0; index4 < PAGEFRAME_NUM; index4++)
    {
        if(page_frame_table[index4].used && (page_frame_table[index4].pfn_pid == pid))
        {
            page_frame_table[index4].used = 0;
        }
    }

    // refresh TLB
    for(index5 = 0; index5 < 32; index5 ++)
    {
        set_cp0_index(index5);
        asm volatile("tlbr");
        tmp_entryhi = get_cp0_entryhi();
        tmp_asid = tmp_entryhi & 0xff;
        if(tmp_asid == pid)
        {            
            tmp_entrylo0 = get_cp0_entrylo0();
            tmp_entrylo1 = get_cp0_entrylo1();

            tmp_entryhi  = (tmp_entryhi & 0xffffe000); // let asid = 0;
            tmp_entrylo0 = (tmp_entrylo0 & 0xfffffffd); // let v = 0;
            tmp_entrylo1 = (tmp_entrylo1 & 0xfffffffd); // let v = 0;
            
            set_cp0_entryhi(tmp_entryhi);
            set_cp0_entrylo0(tmp_entrylo0);
            set_cp0_entrylo1(tmp_entrylo1);
            set_cp0_pagemask(0);
            //set_cp0_index();
            asm volatile("tlbwi");
        }
    }
*/


    do_scheduler(); // need it?
}

void do_exit()
{
    int index1, index2, index3, index4, index5;
    //uint32_t tmp_entryhi, tmp_entrylo0, tmp_entrylo1, tmp_asid;
        
    for(index1 = 0; index1 < NUM_MAX_TASK; index1++)
    {
        if(current_running == &pcb[index1])
            break;
    }

    if(index1 == NUM_MAX_TASK)
    {
        printk("In do_wait(). ERROR: process not exists.\n");
        while(1);
    }

    
    for(index2 = 0; index2 < NUM_MAX_LOCK; index2++)
    {
        if(current_running->target_lock_id[index2] == GET_LOCK)
            do_mutex_lock_release(&lock_array[index2], current_running);
    }

    // release wait_queue
    do_unblock_all(&(current_running->wait_queue));

    // change status
    current_running->status = TASK_EXITED;

    // pop from the ready_queue
    //queue_remove(&ready_queue, current_running);

    // release stack space
    current_running->kernel_context.regs[29] = STACK_TOP - 2*index1 * STACK_SIZE;
    current_running->kernel_stack_top = STACK_TOP - 2*index1 * STACK_SIZE;
    current_running->user_context.regs[29] = pcb[index1].kernel_stack_top - STACK_SIZE;
    current_running->user_stack_top = pcb[index1].kernel_stack_top - STACK_SIZE;
/*
    // refresh page table
    for(index3 = 0; index3 < PGTABLE_NUM; index3++)
    {
        if(page_table[index3].pte_pid == current_running->pid)
        {
            (page_table[index3].ctrl) &= (~PTE_V); // valid = 0;
            (page_table[index3].ctrl) &= (~PTE_R); // reference = 0;
            page_table[index3].pte_pid = 0;
            
        }
    }

    // refresh page frame
    for(index4 = 0; index4 < PAGEFRAME_NUM; index4++)
    {
        if(page_frame_table[index4].used && (page_frame_table[index4].pfn_pid == current_running->pid))
        {
            page_frame_table[index4].used = 0;
        }
    }

    // refresh TLB
    for(index5 = 0; index5 < 32; index5 ++)
    {
        set_cp0_index(index5);
        asm volatile("tlbr");
        tmp_entryhi = get_cp0_entryhi();
        tmp_asid = tmp_entryhi & 0xff;
        if(tmp_asid == current_running->pid)
        {            
            tmp_entrylo0 = get_cp0_entrylo0();
            tmp_entrylo1 = get_cp0_entrylo1();

            tmp_entryhi  = (tmp_entryhi & 0xffffe000); // let asid = 0;
            tmp_entrylo0 = (tmp_entrylo0 & 0xfffffffd); // let v = 0;
            tmp_entrylo1 = (tmp_entrylo1 & 0xfffffffd); // let v = 0;
            
            set_cp0_entryhi(tmp_entryhi);
            set_cp0_entrylo0(tmp_entrylo0);
            set_cp0_entrylo1(tmp_entrylo1);
            set_cp0_pagemask(0);
            //set_cp0_index();
            asm volatile("tlbwi");
        }
    }
*/
    do_scheduler(); // need it?
}

void do_wait(pid_t pid)
{
    pcb_t * tmp_pointer = NULL;
    int index1, index3;
    current_running->waiting_pid = pid;
    current_running->status = TASK_BLOCKED;
    
    for(index1 = 0; index1 < NUM_MAX_TASK; index1++)
    {
        if(pcb[index1].pid == pid)
        {
            tmp_pointer = &pcb[index1];
            break;
        }
    }

    if(tmp_pointer == NULL)
    {
        printk("In do_wait(). ERROR: PID not exists.\n");
        while(1);
    }

    current_running->block_in_queue = &(tmp_pointer->wait_queue);
    queue_push(&(tmp_pointer->wait_queue), current_running);


    do_scheduler(); // need it?
}

pid_t do_getpid()
{
    return current_running->pid;
}
