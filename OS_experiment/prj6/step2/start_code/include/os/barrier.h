#ifndef INCLUDE_BARRIER_H_
#define INCLUDE_BARRIER_H_

#include "queue.h"

typedef struct barrier
{
    queue_t barrier_queue;
    int arrived_num;
    int goal;
} barrier_t;

void do_barrier_init(barrier_t *barrier, int num_task);
void do_barrier_wait(barrier_t *barrier);

#endif