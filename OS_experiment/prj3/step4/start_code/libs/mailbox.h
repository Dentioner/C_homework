#ifndef INCLUDE_MAIL_BOX_
#define INCLUDE_MAIL_BOX_

#include "cond.h"

#define MAX_MAILBOX_NAME_LENGTH 50
#define MSG_BUFFER_LENGTH 100 


typedef struct mailbox
{
    char box_name[MAX_MAILBOX_NAME_LENGTH];
    condition_t box_not_full;
    condition_t box_not_empty;
    mutex_lock_t * box_lock;
    int ref_counter;
    //int msg_buffer[MSG_BUFFER_LENGTH];    
    char msg_buffer[MSG_BUFFER_LENGTH];
    //int box_index;
    int box_index_read;// queue_front
    int box_index_write;// queue_rear
    int buffer_counter;// queue_counter
    // buffer_counter is the number of messages, not the byte length of messages.
    int used;
} mailbox_t;


void mbox_init();
mailbox_t *mbox_open(char *);
void mbox_close(mailbox_t *);
void mbox_send(mailbox_t *, void *, int);
void mbox_recv(mailbox_t *, void *, int);

#endif
