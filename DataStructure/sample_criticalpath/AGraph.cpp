#include "AGraph.h"

void CreateDirectedGraph(AGraph *g){
struct edgeInput{
    char x,y;
    int w;
};
struct edgeInput EInput[11]={
    'A','B',6,
    'A','C',4,
    'A','D',5,
    'B','E',1,
    'C','E',1,
    'D','F',2,
    'E','G',9,
    'E','H',7,
    'G','I',2,
    'H','I',4,
    'F','H',4,
};
char x,y;int w;
int i; NodeLink *p,*q,*s;

g->vexnum=9;g->edgenum=11;
for(i=0;i<g->vexnum;i++) {//初始化邻接表
    g->v[i].vertex = 'A'+i;
    g->v[i].first=NULL;
    }
for(i=0;i<g->edgenum;i++){
    x=EInput[i].x;y=EInput[i].y;w=EInput[i].w;
    //生成表结点并插入邻接表
    s=(NodeLink *)malloc(sizeof(NodeLink));
    s->vindex= y-'A';
    s->info=w;
    if(g->v[x-'A'].first == NULL) {
        g->v[x-'A'].first =s;
        s->next = NULL;
        }
    else {
        p=g->v[x-'A'].first;q=p->next;
        while (q!=NULL){
            p=q;q=q->next;
        }
        p->next=s;s->next=q;
    }
}
}

void ListGraph(AGraph *g) {
int i; NodeLink *p;
for(i=0;i<g->vexnum;i++){
    printf("%d:%c--->",i,g->v[i].vertex);
    p=g->v[i].first;
    while(p) {
        printf("%3d,%d",p->vindex,p->info);
        p=p->next;}
    printf("\n");
    }
}

void FindIndegree(AGraph *g,int indegree[MAX_VERTEX_NUM]){
int i; NodeLink *p;

for(i=0;i<g->vexnum;i++){
    p=g->v[i].first;
    while(p) {
        indegree[p->vindex]++;
        p=p->next;
        }
}
}
// orderResult 堆栈，存放 顶点的排序结果
// ve 存放 顶点/事件的最早发生时间
Status TopologicalOrder(AGraph *g, char orderResult[],int ve[])
{
int indegree[MAX_VERTEX_NUM]; //存放顶点的入度
char stack[MAX_VERTEX_NUM];
int top=0;//存放入度为0的顶点编号
int i,j,count;
NodeLink *p;

for(i=0;i<MAX_VERTEX_NUM;i++) 
    indegree[i]=0;
FindIndegree(g,indegree);
for(i=g->vexnum-1;i>=0;i--)
    if(indegree[i]==0) {//入度为0者进栈
        stack[top]=i;
        top++;
        }

for(i=0;i<g->vexnum;i++) 
    ve[i]=0;
count=0; //对输出顶点计数
while(top!=0){
    top--;
    i=stack[top];
    orderResult[count]=g->v[i].vertex;
    count++;
    for(p=g->v[i].first;p;p=p->next){
        j=p->vindex;// 对i号顶点的每个邻接点的入度减1
        if(--indegree[j]==0){// 若入度减为0，则入栈
            stack[top]=j;
            top++;
            }
        if(ve[j]<ve[i] + p->info)
            ve[j]=ve[i]+ p->info;
        }
}
if(count<g->vexnum) 
    return ERROR;// 该图有回路
else 
    return OK;
}
