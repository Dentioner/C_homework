#ifndef LINKEDQUEUE_H_INCLUDED
#define LINKEDQUEUE_H_INCLUDED

#define OK 1
#define ERROR 0

typedef int Status;
typedef int QElemType;

typedef struct Node {
    QElemType data;
    struct Node *next;
}QNode;
typedef struct {
    QNode *front;
    QNode *rear;
}LinkedQueue;

Status InitQueue(LinkedQueue *lq);
int GetLen(LinkedQueue *lq);
int IsQueueEmpty(LinkedQueue *lq);
Status GetFront(LinkedQueue *lq,QElemType *e);
Status Enqueue(LinkedQueue *lq,QElemType e);
Status Dequeue(LinkedQueue *lq,QElemType *e);
void PrintQueue(LinkedQueue *lq);


#endif // LINKEDQUEUE_H_INCLUDED
