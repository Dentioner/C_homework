/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "irq.h"
#include "test.h"
#include "stdio.h"

#include "screen.h"
#include "common.h"
#include "syscall.h"

#include "lock.h"	// part2

static void init_memory();

extern pid_t process_id;
extern pcb_t pcb[NUM_MAX_TASK];//task array
//extern unsigned int pcb_array_length;

extern queue_t ready_queue;
//extern queue_t ready_queue_p1;
//extern queue_t ready_queue_p2;
//extern queue_t ready_queue_p3;
//extern queue_t block_queue;
extern mutex_lock_t block_queue_array[NUM_MUTEX_LOCK];

extern int num_lock_tasks;
//extern mutex_lock_t mutex_lock;
//mutex_lock_t mutex_lock;

extern queue_t sleep_queue;
extern int num_timer_tasks;
extern int num_sched2_tasks;

static void init_memory()
{
	//init_page_table(); 
	//In task1&2, page table is initialized completely with address mapping, but only virtual pages in task3.
	init_TLB();		//only used in P4 task1
	//init_swap();		//only used in P4 bonus: Page swap mechanism
}


static void init_pcb()
{
	int i, j;
	for(i = 0; i < NUM_MAX_TASK; i++)
	{
		memset(pcb[i], 0 , sizeof(pcb_t));
		//pcb[i].status = TASK_EXITED;
		pcb[i].status = TASK_UNUSED;
		pcb[i].first_running = 1;
		pcb[i].type = KERNEL_PROCESS;
		queue_init(&(pcb[i].wait_queue));

	}

		
	pcb[1].kernel_context.regs[29] = STACK_TOP - 2 * STACK_SIZE;	//sp
	pcb[1].kernel_context.regs[31] = (uint32_t)handle_int + 0x14;			//ra
	

	pcb[1].kernel_stack_top = STACK_TOP - 2 * STACK_SIZE;;

	pcb[1].type = KERNEL_THREAD;
	pcb[1].status = TASK_READY;
	pcb[1].kernel_context.cp0_status = CP0_STATUS_INIT;

	pcb[1].user_context.regs[29] = pcb[1].kernel_stack_top - STACK_SIZE;
	pcb[1].user_context.regs[31] = (uint32_t)&test_shell;
	pcb[1].user_stack_top = pcb[1].kernel_stack_top - STACK_SIZE;
	pcb[1].user_context.cp0_status = CP0_STATUS_INIT;
	pcb[1].user_context.cp0_epc = (uint32_t)&test_shell; // because "eret" is after "RESTORE_CONTEXT(USER)"
	pcb[1].running_counter = 1;
	pcb[1].pid = 1;
	queue_push(&ready_queue, &pcb[1]);



	// let pcb[0] become the first process
	
	// init_pcb() works accorrding to the definition style of pcb structure 
	/* register context */
	pcb[0].kernel_context.regs[29] = STACK_TOP;	// sp <= stack top
	pcb[0].kernel_stack_top = STACK_TOP;

	// accorrding to lecture2.pdf, user shares the same address with kernel
	pcb[0].user_context.regs[29] = pcb[0].kernel_context.regs[29];
//	pcb[0].user_stack_top = pcb[0].kernel_stack_top;
	
	pcb[0].user_stack_top = STACK_TOP - STACK_SIZE/2;
	
    /* previous, next pointer */
	pcb[0].prev = NULL;
	pcb[0].next = NULL;
    /* process id */
	pcb[0].pid = 0;
    /* kernel/user thread/process */
    pcb[0].type = KERNEL_PROCESS;
	/* BLOCK | READY | RUNNING */
	pcb[0].status = TASK_RUNNING;
    /* cursor position */ 


	pcb[0].priority = P1;


	pcb[0].first_running = 0;

	//pcb[0].kernel_context.regs[31] = (uint32_t)handle_int; // I'm not sure 
	//	TODO:
	//	what about pcb[0].kernel_context.cp0_status ?
	//	what about pcb[0].kernel_context.cp0_cause ?

	current_running = &pcb[0];


//	printk("test. i = %d\n", i);

	return;
}


static void init_queue()
{
	queue_init(&ready_queue); // part1
	// queue_init(&block_queue);
	//int i;
	//for (i = 0; i < NUM_MUTEX_LOCK; i++ )	// part2
	//{
	//	do_mutex_lock_init(block_queue_array[i]);
	//}
	// queue_init(&ready_queue_p1); // part3
	// queue_init(&ready_queue_p2); // part3
	// queue_init(&ready_queue_p3); // part3
	


	//do_mutex_lock_init(&mutex_lock);

	queue_init(&sleep_queue);
	return;
}


static void init_exception_handler()
{
	//TODO
	//Add TLB_exception_handler in project_4.
}

static void init_exception() // init the interrupt system 
{
	// 1. Get CP0_STATUS
	// reference: https://blog.csdn.net/comcat/article/details/1557963
	uint32_t my_cp0_status;
	/*
	asm volatile(
		"mfc0	%0, CP0_STATUS\n\t"
		:"=r"(my_cp0_status)
		:
		);
	*/
	my_cp0_status = get_cp0_status();
//	printk("test: step1 finished\n");
	// 2. Disable all interrupt
	
	//asm volatile("CLI");
	disable_all_interrupt();

//	printk("test: step2 finished\n");
	// 3. Copy the level 2 exception handling code to 0x80000180
	memcpy(BEV0_EBASE + BEV0_OFFSET, exception_handler_begin, exception_handler_end - exception_handler_begin);
//	memcpy(BEV1_EBASE + BEV1_OFFSET, exception_handler_begin, exception_handler_end - exception_handler_begin);
	
//	printk("test: step3 finished\n");
	// 4. reset CP0_COMPARE & CP0_COUNT & CP0_STATUS register
	my_cp0_status |= 0x10008000; // let status[28] = 1, status[15] = 1 and status[0] = 1
	/*
	asm volatile(
		"mtc0	$0, CP0_COUNT\n\t"
		"li		k0, TIMER_INTERVAL\n\t"
		"mtc0	k0, CP0_COMPARE\n\t"
		"mtc0	%0, CP0_STATUS\n\t"
		:
		:"r"(my_cp0_status)
		);
	*/
	reset_timer();
	set_cp0_status(my_cp0_status);
//	printk("test: step4 finished.\n");

}

static void init_syscall(void)
{
	// init system call table.
	
	//type convert : int(*)() <= it is a kind of funtion type with integer return and void input
	syscall[SYSCALL_SLEEP] = (int (*)())&do_sleep;
	syscall[SYSCALL_PS] = (int (*)())&do_process_show;
	syscall[SYSCALL_SPAWN] = (int (*)())&do_spawn;
	syscall[SYSCALL_KILL] = (int (*)())&do_kill;
	syscall[SYSCALL_EXIT] = (int (*)())&do_exit;
	syscall[SYSCALL_WAIT] = (int (*)())&do_wait;

	syscall[SYSCALL_BLOCK] = (int (*)())&do_block;
	syscall[SYSCALL_UNBLOCK_ONE] = (int (*)())&do_unblock_one;
	syscall[SYSCALL_UNBLOCK_ALL] = (int (*)())&do_unblock_all;

	syscall[SYSCALL_SEMAPHORE_INIT] = (int (*)())&do_semaphore_init;
	syscall[SYSCALL_SEMAPHORE_UP] = (int (*)())&do_semaphore_up;
	syscall[SYSCALL_SEMAPHORE_DOWN] = (int (*)())&do_semaphore_down;

	syscall[SYSCALL_CONDITION_INIT] = (int (*)())&do_condition_init;
	syscall[SYSCALL_CONDITION_WAIT] = (int (*)())&do_condition_wait;
	syscall[SYSCALL_CONDITION_SIGNAL] = (int (*)())&do_condition_signal;
	syscall[SYSCALL_CONDITION_BROADCAST] = (int (*)())&do_condition_broadcast;

	syscall[SYSCALL_WRITE] = (int (*)())&screen_write;
//	syscall[SYSCALL_READ] = (int (*)())&I_donnot_know;
	syscall[SYSCALL_CURSOR] = (int (*)())&screen_move_cursor;
	syscall[SYSCALL_REFLUSH] = (int (*)())&screen_reflush;
	syscall[SYSCALL_CLEAR] = (int (*)())&screen_clear;

	syscall[SYSCALL_BARRIER_INIT] = (int (*)())&do_barrier_init;
	syscall[SYSCALL_BARRIER_WAIT] = (int (*)())&do_barrier_wait;

	syscall[SYSCALL_GETPID] = (int (*)())&do_getpid;

	syscall[SYSCALL_MUTEX_LOCK_INIT] = (int (*)())&do_mutex_lock_init;
	syscall[SYSCALL_MUTEX_LOCK_ACQUIRE] = (int (*)())&do_mutex_lock_acquire;
	syscall[SYSCALL_MUTEX_LOCK_RELEASE] = (int (*)())&do_mutex_lock_release;

	
		
	
	
		
	
	
}

// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
void __attribute__((section(".entry_function"))) _start(void)
{
	// Close the cache, no longer refresh the cache 
	// when making the exception vector entry copy
	asm_start();

	// init interrupt (^_^)
	init_exception();
	printk("> [INIT] Interrupt processing initialization succeeded.\n");


	// init virtual memory
	init_memory();
	printk("> [INIT] Virtual memory initialization succeeded.\n");

	// init system call table (0_0)
	init_syscall();
	printk("> [INIT] System call initialized successfully.\n");

	// init Process Control Block (-_-!)
	init_queue();
	


	init_pcb();
	printk("> [INIT] PCB initialization succeeded.\n");

	// init screen (QAQ)
	init_screen();
	printk("> [INIT] SCREEN initialization succeeded.\n");

/*	printk("\ntest\n");
        printk("status = 0x%x\n", get_cp0_status());
        printk("count = %u\n", get_count());
        printk("compare = %u\n", get_compare());
*/

//	printk("test. I'm here.\n");
	// TODO Enable interrupt
	open_all_interrupt();

//	printk("test.\n");
/*	int index;
	printk("test\n");
	printk("status = 0x%x\n", get_cp0_status());
	printk("count = %u\n", get_count());
	printk("compare = %u\n", get_compare());

	for(index = 0; index < 1000; index++)
		;
	printk("\n");
	printk("test\n");
        printk("status = 0x%x\n", get_cp0_status());
        printk("count = %u\n", get_count());
        printk("compare = %u\n", get_compare());


*/
	while (1)
	{
		// (QAQQQQQQQQQQQ)
		// If you do non-preemptive scheduling, you need to use it to surrender control
		// do_scheduler();
	};
	return;
}
