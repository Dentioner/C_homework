#include "string.h"
#include "mailbox.h"
#include "syscall.h"

//#define MAX_NUM_BOX 32
#define MAX_NUM_BOX 16


static mailbox_t mboxs[MAX_NUM_BOX];

mutex_lock_t biglock; //test

void mbox_init()
{
    int index1;
    mutex_lock_init(&biglock); // test
    for(index1 = 0; index1< MAX_NUM_BOX; index1++)
    {
    //bzero(&mboxs[index1], sizeof(mailbox_t));
	mboxs[index1].box_name[0] = '\0';
	mboxs[index1].ref_counter = 0;  
	mboxs[index1].msg_buffer[0] = '\0';
	mboxs[index1].box_index_read = 0;
    mboxs[index1].box_index_write = 0;
	//mboxs[index1].used = 0;
    mboxs[index1].buffer_counter = 0;
	
	condition_init(&mboxs[index1].box_not_full);
    condition_init(&mboxs[index1].box_not_empty);
        
        //queue_init(&mboxs[index1].box_not_full.condition_queue);
        //queue_init(&mboxs[index1].box_not_empty.condition_queue);
        
    mboxs[index1].box_lock = &lock_array[index1];

    mutex_lock_init(mboxs[index1].box_lock);
        //mboxs[index1].box_lock->status = UNLOCKED;
        //queue_init(&(mboxs[index1].box_lock->block_queue));
        //mboxs[index1].box_lock->locked_process_pid = 0;
        //mboxs[index1].box_lock->address_of_locked_process = NULL;
    }
}

void release_mailbox(mailbox_t * box)
{
    condition_broadcast(&box->box_not_full);
    condition_broadcast(&box->box_not_empty);
    box->box_lock->address_of_locked_process = NULL;
    box->box_lock->locked_process_pid = 0;
    box->box_lock->status = UNLOCKED;
    sys_unblock_all(&box->box_lock->block_queue);
    box->ref_counter = 0;
    bzero(box->box_name, MAX_MAILBOX_NAME_LENGTH * sizeof(char));
    bzero(box->msg_buffer, MSG_BUFFER_LENGTH * sizeof(char));
    //box->box_index = 0;
    box->box_index_read = 0;
    box->box_index_write = 0;
    //box->used = 0;
    box->buffer_counter = 0;
    //bzero(box, sizeof(mailbox_t));
    //condition_init(&box->box_not_full);
    //condition_init(&box->box_not_empty);
    
}

mailbox_t *mbox_open(char *name)
{
    mutex_lock_acquire(&biglock); // test

    int index1;
    for(index1 = 0; index1 < MAX_NUM_BOX; index1++)
    {
        if(!strcmp(name, mboxs[index1].box_name))
        {
            mboxs[index1].ref_counter++;
            mutex_lock_release(&biglock, current_running); // test
            return &mboxs[index1];
        }
    }

// do not find an exists mailbox that has the same name
    for(index1 = 0; index1 < MAX_NUM_BOX; index1++)
    {
        //if(!mboxs[index1].used)
        if(!mboxs[index1].ref_counter) 
        {
            break;
        }
    }

    if(index1 < MAX_NUM_BOX)// find an idle mailbox
    {
        //mboxs[index1].used = 1;
        mboxs[index1].ref_counter++;
        //memcpy(mboxs[index1].box_name, name, sizeof(*name)); 
        strcpy(mboxs[index1].box_name, name);
        mutex_lock_release(&biglock, current_running); // test
        return &mboxs[index1];
    }
    else// do not have enough mailbox
    {
        printf("Error. Donot have enough mailbox.\n");
        while(1);
    }
    mutex_lock_release(&biglock, current_running); // test
}

void mbox_close(mailbox_t *mailbox)
{
    mutex_lock_acquire(&biglock); // test
    mailbox->ref_counter--;
    if(mailbox->ref_counter == 0)
    {
        release_mailbox(mailbox);
    }
    mutex_lock_release(&biglock, current_running); // test
}

void mbox_send(mailbox_t *mailbox, void *msg, int msg_length)
{
    int array_length, empty_length;
    int index1;
    char * ch_msg = msg;   
    mutex_lock_acquire(mailbox->box_lock);
    array_length = (mailbox->box_index_write - mailbox->box_index_read + MSG_BUFFER_LENGTH)%MSG_BUFFER_LENGTH;
    empty_length = (!mailbox->buffer_counter)? MSG_BUFFER_LENGTH : MSG_BUFFER_LENGTH - array_length;
    while(empty_length <= msg_length)
    {
        condition_wait(mailbox->box_lock, &mailbox->box_not_full);
    }
    //add msg to buffer
    for(index1 = 0; index1 < msg_length; index1++)
    {
        mailbox->msg_buffer[mailbox->box_index_write] = *ch_msg++;
        mailbox->box_index_write = (mailbox->box_index_write + 1)%MSG_BUFFER_LENGTH;
    }
    //mailbox->box_index += msg_length;
    mailbox->buffer_counter++;

    //condition_signal(&mailbox->box_not_empty);
    
    condition_broadcast(&mailbox->box_not_empty);
    mutex_lock_release(mailbox->box_lock, current_running);
}

void mbox_recv(mailbox_t *mailbox, void *msg, int msg_length)
{
    int array_length, empty_length;
    int index1;
    char * ch_msg = msg;    
    mutex_lock_acquire(mailbox->box_lock);
    array_length = (mailbox->box_index_write - mailbox->box_index_read + MSG_BUFFER_LENGTH)%MSG_BUFFER_LENGTH;
    empty_length = (!mailbox->buffer_counter)? MSG_BUFFER_LENGTH : MSG_BUFFER_LENGTH - array_length;
    
    
    while(!mailbox->buffer_counter)
    {
        condition_wait(mailbox->box_lock, &mailbox->box_not_empty);
    }
    //remove msg from buffer
    
    //mailbox->box_index -= msg_length;
    /*if(mailbox->box_index < 0)
    {
        printf("Error in mbox_recv. not enough index.\n");
        while(1);
    }
    memcpy(msg, &mailbox->msg_buffer[mailbox->box_index], msg_length);*/
    for(index1 = 0; index1 < msg_length; index1++)
    {
        *ch_msg++ = mailbox->msg_buffer[mailbox->box_index_read];
        mailbox->box_index_read = (mailbox->box_index_read + 1)%MSG_BUFFER_LENGTH;
    }
    

    mailbox->buffer_counter--;
    //condition_signal(&mailbox->box_not_empty);
    
    condition_broadcast(&mailbox->box_not_full);
    mutex_lock_release(mailbox->box_lock, current_running);
}
