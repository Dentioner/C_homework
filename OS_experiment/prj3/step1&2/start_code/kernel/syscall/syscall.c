#include "lock.h"
#include "sched.h"
#include "common.h"
#include "screen.h"
#include "syscall.h"

void system_call_helper(int fn, int arg1, int arg2, int arg3)
{
    // syscall[fn](arg1, arg2, arg3)
    //int ret_value;

    //debug
    /*
    if(fn < 0 || fn > NUM_SYSCALLS - 1)
    {
        printk("error. wrong syscall number. fn = %d\t, arg1 = %d\t, arg2 = %d\t, arg3 %d\n", fn, arg1, arg2, arg3);
	    printk("current_running is : 0x%x\n", current_running);
        while(1);
    }*/
	
    //ret_value = syscall[fn](arg1, arg2, arg3);
    //current_running->user_context.regs[2] = ret_value; // $v0 <= ret_value
	current_running->user_context.cp0_epc = current_running->user_context.cp0_epc + 4;
    syscall[fn](arg1, arg2, arg3);
}

void sys_sleep(uint32_t time)
{
    invoke_syscall(SYSCALL_SLEEP, time, IGNORE, IGNORE);
}

void sys_process_show()
{
    invoke_syscall(SYSCALL_PS, IGNORE, IGNORE, IGNORE);
}

void sys_spawn(task_info_t *task)
{
    invoke_syscall(SYSCALL_SPAWN, (int)task, IGNORE, IGNORE);
}

void sys_kill(pid_t pid)
{
    invoke_syscall(SYSCALL_KILL, pid, IGNORE, IGNORE);
}

void sys_exit()
{
    invoke_syscall(SYSCALL_EXIT, IGNORE, IGNORE, IGNORE);
}

void sys_waitpid(pid_t pid)
{
    invoke_syscall(SYSCALL_WAIT, pid, IGNORE, IGNORE);
}

void sys_block(queue_t *queue)
{
    invoke_syscall(SYSCALL_BLOCK, (int)queue, IGNORE, IGNORE);
}

void sys_unblock_one(queue_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ONE, (int)queue, IGNORE, IGNORE);
}

void sys_unblock_all(queue_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ALL, (int)queue, IGNORE, IGNORE);
}

void sys_write(char *buff)
{
    invoke_syscall(SYSCALL_WRITE, (int)buff, IGNORE, IGNORE);
}

void sys_reflush()
{
    invoke_syscall(SYSCALL_REFLUSH, IGNORE, IGNORE, IGNORE);
}

void sys_move_cursor(int x, int y)
{
    invoke_syscall(SYSCALL_CURSOR, x, y, IGNORE);
}

void sys_clear(int line1, int line2)
{
    invoke_syscall(SYSCALL_CLEAR, line1, line2, IGNORE);
}

void mutex_lock_init(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_INIT, (int)lock, IGNORE, IGNORE);
}

void mutex_lock_acquire(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_ACQUIRE, (int)lock, IGNORE, IGNORE);
}

void mutex_lock_release(mutex_lock_t *lock, pcb_t * process)
{
//	printk("test: before invoke.\n");
//	while(1);
//	printk("%d\n", SYSCALL_MUTEX_LOCK_RELEASE);
//	while(1);
 invoke_syscall(SYSCALL_MUTEX_LOCK_RELEASE, (int)lock, (int)process, IGNORE);
//	printk("test: a = %d\n", a);
//	while(1);
	    
}


