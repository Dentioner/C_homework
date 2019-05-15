#include "AGraph.h"

void CreateGraph(AGraph *g,int n,int m)
{int i,e=0; NodeLink *p,*q,*s; char x,y;

g->vexnum=n; g->kind=m;
for(i=0;i<n;i++) {//初始化邻接表
    g->v[i].vertex = 'A'+i;
    g->v[i].first=NULL;}
printf("Input edges x-->y:");
scanf("%c%c",&x,&y);
while(x!='X' && y!='X'){
    e++;
    //生成表结点并插入邻接表
    s=(NodeLink *)malloc(sizeof(NodeLink));
    s->vindex= y-'A';
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
if(!g->kind){ //无向图
    s=(NodeLink *)malloc(sizeof(NodeLink));
    s->vindex = x-'A';
    if(g->v[y-'A'].first == NULL) {
        g->v[y-'A'].first =s;s->next = NULL;}
    else {
        p=g->v[y-'A'].first; q=p->next;
        while(q!=NULL){
            p=q;q=q->next;}
        p->next =s;s->next=q;
        }
}
scanf(" %c%c",&x,&y);
}
g->edgenum =e;
}

void ListGraph(AGraph *g) {
int i; NodeLink *p;
for(i=0;i<g->vexnum;i++){
    printf("%d:%c--->",i,g->v[i].vertex);
    p=g->v[i].first;
    while(p) {
        printf("%3d",p->vindex);
        p=p->next;}
    printf("\n");
    }
}

int LocateVex(AGraph *g,ElemType u){
int k;
for(k=0;k<g->vexnum;k++)
    if (g->v[k].vertex == u) return k;
return -1;
}

ElemType GetVex(AGraph *g,int x){
    return g->v[x].vertex;
}
