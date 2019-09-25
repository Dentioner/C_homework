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
//queue_t block_queue;    //it may not be used in part1
mutex_lock_t block_queue_array[NUM_MUTEX_LOCK]; // part2 mutex lock
extern mutex_lock_t mutex_lock;

pid_t total_pid = 1; // this is used to init a new pcb and give it a pid

static void check_sleeping()
{
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
            queue_push(&ready_queue, current_running);

    }
    current_running = queue_dequeue(&ready_queue);


    // because queue is FIFO, so the pcb that current_running pointed earlier
    // will not be pushed out firstly


    //   ������û����mutex�Ĳ�����test_lock1������е�ʱ��д��
    //   ȫ��ľ��
    /*
    while(!queue_is_empty(&ready_queue))
    {    
        temp_process_pointer = queue_dequeue(&ready_queue);
        not_be_blocked = 1;// this variable will be initialized when another process is poped out
        for (i = 0; i < NUM_MUTEX_LOCK; i++)
        {
        //  �������ѭ����һЩע��
        //  ���￼���˶��������������ĳ���������̷�����һ������������
        //  �ǾͲ���ѭ���ˣ���Ϊ��ʱ����ȱʧ���������Ӧ����Դ���޷�����ִ�й���
        //  ���ͱ���block����ô��˳��block���������queue����
        //  ������������Ǹ���������ÿһ�������õ��ˣ���ô���Ų��ᱻblock
        //  Ҳ����˵���������ĳ��������˵����ִ������������ѭ����û����ǰ����
        //  ��ô��˵����ÿһ�������õ��ˣ�������current_runningҪת��Ľ���

            if(!do_mutex_lock_acquire(&block_queue_array[i], temp_process_pointer))
            {
                do_block(&block_queue_array[i], temp_process_pointer);
                not_be_blocked = 0;
                break; // �������/�߳��Ѿ�ûϣ���ˣ���ǰ����forѭ��
            }
            
            
        }

        if(not_be_blocked)
        {
        //  ����ֵ��ע���һ���ǣ��������ཨ����ϵ����䣬�����������Ǹ�forѭ����ִ��
        //  ��Ϊ����ĳ������ֻ�ܻ�ȡһ���ֵ�������һ�����޷���ȡ
        //  ����˵��һ����a��b��c��d�ĸ���������A������a��b������
        //  �������ʱ��c��d������������Bռ����
        //  ����������Ǹ�forѭ������ִ�С�����Aռ����a��b��
        //  �ͻᵼ�½���A����ab��������block
        //  �����Ļ�������BҲ�޷����a��b��������Ҳ����˵���еĽ��̶�û����ȡ���е���
        //  ��Ϊĳ������һ��block�Ľ���ռ�ˣ��������������û�����������һֱû�����Լ��õ����ͷŵ�
        //  ��˱���ȷ��������̿����õ����е���֮��������������е���������ϵ

            for (i = 0; i < NUM_MUTEX_LOCK; i++)
            {
                block_queue_array[i]->locked_process_pid = temp_process_pointer->pid;
                block_queue_array[i]->address_of_locked_process = temp_process_pointer;

                temp_process_pointer->lock_address[i] = block_queue_array[i];
                //temp_process_pointer->status = TASK_RUNNING;
            }    
            
            current_running = temp_process_pointer;
            
            return;
        }


    }

    // if the program returns to upper function on this "return"
    // it means that the current_running has not been changed.
    // in fact, this return will not be executed in theory
    printk("something error WTF?\n");

    */
    //   ������û����mutex�Ĳ�����test_lock1������е�ʱ��д��
    //   ȫ��ľ��

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

/*
void do_block(queue_t *queue, pcb_t* pcb_pointer)
{
    // block the current_running task into the queue
    pcb_pointer->status = TASK_BLOCKED;
    queue_push(queue, pcb_pointer);

}*/

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

/*
void unblock()
{
    pcb_t* pcb_pointer = NULL;
    mutex_lock_t * lock = NULL;
    int i;
    for(i = 0; i < NUM_MUTEX_LOCK; i++)
    {// ���ڵ�ǰrunning�Ľ�����˵���������й�ϵ�����е�������
       lock = current_running->lock_address[i];
       do_unblock_all(&(lock->block_queue));

       lock->address_of_locked_process = NULL;
       lock->locked_process_pid = 0;
       lock->status = UNLOCKED;

       current_running->lock_address[i] = NULL;
    }

    // ��������Щʱ������Ĵ���
    pcb_t * pointer = NULL;
    lock->status = UNLOCKED;
    lock->locked_process_pid = 0;
    lock->address_of_locked_process->lock_address = NULL;
    lock->address_of_locked_process->status = TASK_READY;

    while(!queue_is_empty(&(lock->block_queue)))
    {
        pointer = (pcb_t *)queue_dequeue(&(lock->block_queue));
        pointer->status = TASK_READY;
        queue_push(&ready_queue, pointer);
    }

    queue_push(&ready_queue, lock->address_of_locked_process);
    lock->address_of_locked_process = NULL;
    // ��������Щʱ������Ĵ���
    


    return;
}

*/