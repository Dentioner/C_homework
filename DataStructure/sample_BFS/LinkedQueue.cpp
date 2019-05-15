#include <iostream>
#include <malloc.h>
#include "LinkedQueue.h"
using namespace std;

//链队列的初始化，构造一个空队列
Status InitQueue(LinkedQueue *lq){
lq->front=lq->rear=(QNode *)malloc(sizeof(QNode));
if(!lq->front) return ERROR;
lq->front->next=NULL;
return OK;
}

int GetLen(LinkedQueue *lq){
int i;QNode *p;
p=lq->front->next;i=0;
while(!p){
    i++;p=p->next;}
return i;
}

int IsQueueEmpty(LinkedQueue *lq){
if(lq->front == lq->rear)return 1;
else return 0;
}

Status GetFront(LinkedQueue *lq,QElemType *e){
if(lq->front == lq->rear) return ERROR;
*e=lq->front->next->data;
return OK;
}
//入队：插入元素e为队列的新的队尾元素
Status Enqueue(LinkedQueue *lq,QElemType e){
QNode *p;
p=(QNode *)malloc(sizeof(QNode));
if(!p) return ERROR;
p->data =e; p->next=NULL;
lq->rear->next=p; //修改尾指针
lq->rear=p;
return OK;
}

//出队：若队列不空，则删除队列的队头元素，
//用e返回其值，并返回OK；否则返回ERROR
Status Dequeue(LinkedQueue *lq,QElemType *e){
QNode *p;
if (lq->front == lq->rear) return ERROR; //空队列的话，则出错
p = lq->front->next; //修改头指针
*e = p->data;
lq->front->next =p->next;
if(lq->rear==p) lq->rear=lq->front;//修改尾指针
free(p);
return OK;
}

void PrintQueue(LinkedQueue *lq){
QNode *p;
p=lq->front->next;
while(p!=NULL){
    cout << p->data << " " ;
    p=p->next;
    }
cout <<"\n";
}
