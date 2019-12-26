/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       System call related processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#ifndef INCLUDE_SYSCALL_H_
#define INCLUDE_SYSCALL_H_

#include "type.h"
#include "sync.h"
#include "queue.h"
#include "sched.h"

#define IGNORE 0
#define NUM_SYSCALLS 64

/* define */
#define SYSCALL_SLEEP 2

#define SYSCALL_PS 3
#define SYSCALL_SPAWN 4
#define SYSCALL_KILL 5
#define SYSCALL_EXIT 6
#define SYSCALL_WAIT 7

#define SYSCALL_BLOCK 10
#define SYSCALL_UNBLOCK_ONE 11
#define SYSCALL_UNBLOCK_ALL 12

#define SYSCALL_SEMAPHORE_INIT 13
#define SYSCALL_SEMAPHORE_UP 14
#define SYSCALL_SEMAPHORE_DOWN 15

#define SYSCALL_CONDITION_INIT 16
#define SYSCALL_CONDITION_WAIT 17
#define SYSCALL_CONDITION_SIGNAL 18
#define SYSCALL_CONDITION_BROADCAST 19

#define SYSCALL_WRITE 20
#define SYSCALL_READ 21
#define SYSCALL_CURSOR 22
#define SYSCALL_REFLUSH 23
#define SYSCALL_CLEAR 24

#define SYSCALL_BARRIER_INIT 25
#define SYSCALL_BARRIER_WAIT 26

#define SYSCALL_GETPID 27

#define SYSCALL_MUTEX_LOCK_INIT 30
#define SYSCALL_MUTEX_LOCK_ACQUIRE 31
#define SYSCALL_MUTEX_LOCK_RELEASE 32

#define SYSCALL_INIT_MAC 40
#define SYSCALL_NET_RECV 41
#define SYSCALL_NET_SEND 42
#define SYSCALL_WAIT_RECV_PACKAGE 43

#define SYSCALL_MAKE_FILE_SYSTEM 44
#define SYSCALL_STAT_FILE_SYSTEM 45
#define SYSCALL_CHANGE_DIRECTORY 46
#define SYSCALL_MAKE_DIRECTORY 47
#define SYSCALL_REMOVE_DIRECTORY 48
#define SYSCALL_LIST 49
#define SYSCALL_TOUCH 50
#define SYSCALL_CAT 51

#define SYSCALL_OPEN_FILE 52
#define SYSCALL_READ_FILE 53
#define SYSCALL_WRITE_FILE 54
#define SYSCALL_CLOSE_FILE 55


#define SYSCALL_PRINT_BUFFER 63


/* syscall function pointer */
int (*syscall[NUM_SYSCALLS])(); // this is an array of function pointer

void system_call_helper(int, int, int, int);
extern int invoke_syscall(int, int, int, int);

void sys_sleep(uint32_t);
void sys_process_show();
void sys_spawn(task_info_t * task, unsigned long * argv);
void sys_kill(pid_t pid);
void sys_exit();
void sys_waitpid(pid_t pid);

void sys_block(queue_t *);
void sys_unblock_one(queue_t *);
void sys_unblock_all(queue_t *);

void semaphore_init(semaphore_t *, int);
void semaphore_up(semaphore_t *);
void semaphore_down(semaphore_t *);

void condition_init(condition_t *);
void condition_wait(mutex_lock_t *, condition_t *);
void condition_signal(condition_t *);
void condition_broadcast(condition_t *);

void sys_write(char *);
void sys_move_cursor(int, int);
void sys_reflush();
void sys_clear(int line1, int line2);

void barrier_init(barrier_t *, int);
void barrier_wait(barrier_t *);

pid_t sys_getpid();

void mutex_lock_init(mutex_lock_t *);
void mutex_lock_acquire(mutex_lock_t *);
void mutex_lock_release(mutex_lock_t *, pcb_t*);

uint32_t sys_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr);
void sys_net_send(uint32_t td, uint32_t td_phy);
void sys_init_mac(void);

void sys_wait_recv_package(void);

void sys_mkfs(void);
void sys_statfs(void);
void sys_cd(uint32_t);
void sys_mkdir(uint32_t);
void sys_rmdir(uint32_t);
void sys_ls(void);


void sys_print_buffer(void);

void sys_touch(uint32_t arg_filename);
void sys_cat(uint32_t arg_filename);
void sys_fopen(char *name, int access);
void sys_fread(int fd, char * buff, int size);
void sys_fwrite(int fd, char *buff, int size);
void sys_fclose(int fd);



extern void TLBexception_handler_entry(void);
extern void TLBexception_handler_begin(void);
extern void TLBexception_handler_end(void);
#endif
