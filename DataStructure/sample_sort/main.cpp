#include <iostream>

#include <stdlib.h>
#include "AGraph.h"
using namespace std;

void FindIndegree(AGraph *g,int indegree[MAX_VERTEX_NUM]){
int i; NodeLink *p;

for(i=0;i<g->vexnum;i++){
    p=g->v[i].first;
    while(p) {
        indegree[p->vindex]++;
        p=p->next;
        }
}
//for(i=0;i<g->vexnum;i++)
//    printf("%d ", indegree[i]);
}

Status TopologicalSort(AGraph *g)
{int i,j,count;
char stack[MAX_VERTEX_NUM];int top=0;
int indegree[MAX_VERTEX_NUM]; //存放顶点的入度
NodeLink *p;

for(i=0;i<MAX_VERTEX_NUM;i++) indegree[i]=0;
FindIndegree(g,indegree);
//for(i=0;i<g->vexnum;i++)
for(i=g->vexnum-1;i>=0;i--)
        if(indegree[i]==0) {//入度为0者进栈
            stack[top]=i;
            top++;
            }
count=0; // 对输出顶点计数
printf("\nResults of Topological Sort: ");
while(top!=0){
    top--;
    i=stack[top];
    printf("%c",g->v[i].vertex);
    count++;
    for(p=g->v[i].first;p;p=p->next){
        j=p->vindex;// 对i号顶点的每个邻接点的入度减1
        if(--indegree[j]==0){// 若入度减为0，则入栈
            stack[top]=j;
            top++;
            }
        }
}
if(count<g->vexnum) return ERROR;// 该图有回路
else return OK;
}

int main()
{
AGraph g;int i,j;char c;

printf("Input the number of vertexes and graph kind:");
scanf("%d%d",&i,&j); scanf("%c",&c);
CreateGraph(&g,i,j);
ListGraph(&g);
if(TopologicalSort(&g)) printf("\n ");
else printf("\n The graph has a circle.\n");
system("pause");
return 0;
}
