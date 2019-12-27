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
	int return_value;
    current_running->user_context.cp0_epc = current_running->user_context.cp0_epc + 4;
    return_value = syscall[fn](arg1, arg2, arg3);
    current_running->user_context.regs[2] = return_value; // v0 <= ret
}

void sys_sleep(uint32_t time)
{
    invoke_syscall(SYSCALL_SLEEP, time, IGNORE, IGNORE);
}

void sys_process_show()
{
    invoke_syscall(SYSCALL_PS, IGNORE, IGNORE, IGNORE);
}

void sys_spawn(task_info_t *task, unsigned long * argv)
{
    invoke_syscall(SYSCALL_SPAWN, (int)task, (int)argv, IGNORE);
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


void semaphore_init(semaphore_t *s, int val)
{
    invoke_syscall(SYSCALL_SEMAPHORE_INIT, (int)s, val, IGNORE);
}

void semaphore_up(semaphore_t *s)
{
    invoke_syscall(SYSCALL_SEMAPHORE_UP, (int)s, IGNORE, IGNORE);
}

void semaphore_down(semaphore_t *s)
{
    invoke_syscall(SYSCALL_SEMAPHORE_DOWN, (int)s, IGNORE, IGNORE);
}

void condition_init(condition_t * condition)
{
    invoke_syscall(SYSCALL_CONDITION_INIT, (int)condition, IGNORE, IGNORE);
}

void condition_wait(mutex_lock_t *mutex, condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_WAIT, (int)mutex, (int)condition, IGNORE);
}

void condition_signal(condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_SIGNAL, (int)condition, IGNORE, IGNORE);
}

void condition_broadcast(condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_BROADCAST, (int)condition, IGNORE, IGNORE);
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

void barrier_init(barrier_t *barrier, int num_task)
{
    invoke_syscall(SYSCALL_BARRIER_INIT, (int)barrier, num_task, IGNORE);
}

void barrier_wait(barrier_t *barrier)
{
    invoke_syscall(SYSCALL_BARRIER_WAIT, (int)barrier, IGNORE, IGNORE);
}

pid_t sys_getpid()
{
    invoke_syscall(SYSCALL_GETPID, IGNORE, IGNORE, IGNORE);
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

uint32_t sys_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr)
{
    //invoke_syscall(SYSCALL_NET_RECV, (int)rd, (int)rd_phy, (int)daddr);
    invoke_syscall(SYSCALL_NET_RECV, rd, rd_phy, daddr);
}
void sys_net_send(uint32_t td, uint32_t td_phy)
{
//    invoke_syscall(SYSCALL_NET_SEND, (int)td, (int)td_phy, IGNORE);
    invoke_syscall(SYSCALL_NET_SEND, td, td_phy, IGNORE);
}
void sys_init_mac(void)
{
    invoke_syscall(SYSCALL_INIT_MAC, IGNORE, IGNORE, IGNORE);
}

void sys_wait_recv_package(void)
{
    invoke_syscall(SYSCALL_WAIT_RECV_PACKAGE, IGNORE, IGNORE, IGNORE);
}


void sys_mkfs(void)
{
    invoke_syscall(SYSCALL_MAKE_FILE_SYSTEM, IGNORE, IGNORE, IGNORE);
}

void sys_statfs(void)
{
    invoke_syscall(SYSCALL_STAT_FILE_SYSTEM, IGNORE, IGNORE, IGNORE);
}

void sys_cd(uint32_t filename)
{
    invoke_syscall(SYSCALL_CHANGE_DIRECTORY, filename, IGNORE, IGNORE);
}

void sys_mkdir(uint32_t filename)
{
    invoke_syscall(SYSCALL_MAKE_DIRECTORY, filename, IGNORE, IGNORE);
}

void sys_rmdir(uint32_t filename)
{
    invoke_syscall(SYSCALL_REMOVE_DIRECTORY, filename, IGNORE, IGNORE);
}

void sys_ls(void)
{
    invoke_syscall(SYSCALL_LIST, IGNORE, IGNORE, IGNORE);
}

void sys_touch(uint32_t arg_filename)
{
    invoke_syscall(SYSCALL_TOUCH, arg_filename, IGNORE, IGNORE);
}

void sys_cat(uint32_t arg_filename)
{
    invoke_syscall(SYSCALL_CAT, arg_filename, IGNORE, IGNORE);
}

uint32_t sys_fopen(char *name, int access)
{
    invoke_syscall(SYSCALL_OPEN_FILE, (uint32_t)name, (uint32_t)access, IGNORE);
}


uint32_t sys_fread(int fd, char * buff, int size)
{
    invoke_syscall(SYSCALL_READ_FILE, (uint32_t)fd, (uint32_t)buff, (uint32_t)size);
}

uint32_t sys_fwrite(int fd, char *buff, int size)
{
    invoke_syscall(SYSCALL_WRITE_FILE, (uint32_t)fd, (uint32_t)buff, (uint32_t)size);
}

void sys_fclose(int fd)
{
    invoke_syscall(SYSCALL_CLOSE_FILE, (uint32_t)fd, IGNORE, IGNORE);
}


void sys_print_buffer(void)
{
    invoke_syscall(SYSCALL_PRINT_BUFFER, IGNORE, IGNORE, IGNORE);
}


