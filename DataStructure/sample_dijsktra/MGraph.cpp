#include <stdio.h>
#include "MGraph.h"

//Undirected graph:kind=0 Directed graph:kind=1
void CreateGraph(MGraph *g) {
typedef struct Edge {
    int i,j,w;
} Edge;
int i,j,w,k;

Edge A[]={
1,2,5,
0,2,10,
0,4,30,
0,5,100,
2,3,50,
3,5,10,
4,3,20,
4,5,60,
};
g->vexnum=6;
g->arcnum=8;

for(i=0;i<g->vexnum;i++)
    g->vexs[i]='A'+i;
for(i=0;i<g->vexnum;i++)
    for(j=0;j<g->vexnum;j++)
        g->arcs[i][j]=INFINITY;
for(k=0;k<g->arcnum;k++){
    i=A[k].i;j=A[k].j;w=A[k].w;
    g->arcs[i][j]=w;
}
}

void ListGraph(MGraph *g){
int i,j;
for(i=0;i<g->vexnum;i++) {
    printf("%6c---",g->vexs[i]);
    for(j=0;j<g->vexnum;j++)
        printf("%5d",g->arcs[i][j]);
    printf("\n");
    }
}
