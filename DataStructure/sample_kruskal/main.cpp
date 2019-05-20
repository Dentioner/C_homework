#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "EGraph.h"
using namespace std;

int x[11][3]= {
    1,2,19,
    2,3,5,
    1,7,18,
    1,5,14,
    2,5,12,
    2,4,7,
    3,4,3,
    7,5,16,
    5,4,8,
    4,6,21,
    7,6,27,
};
void CreateGraph (EGraph *g){
int i;
g->vexnum=7; g->edgenum=11;
for(i=0;i<g->vexnum;i++) g->vexlist[i]=i+1;
for(i=0;i<g->edgenum;i++) {
    g->edgelist[i].ivex=x[i][0];
    g->edgelist[i].jvex=x[i][1];
    g->edgelist[i].weight=x[i][2];
}

/*char c;
printf("Input vertex number and edge number in the graph:\n");
scanf("%d %d",&g->vexnum,&g->edgenum); scanf("%c",&c);
for(i=0;i<g->vexnum;i++)
    scanf(" %d",&g->vexlist[i]);
printf("Input edge info: starting vertex,ending vertex,weight:\n");
for(i=0;i<g->edgenum;i++) {
    scanf(" %d %d %d",&g->edgelist[i].ivex,&g->edgelist[i].jvex,&g->edgelist[i].weight);
    }
*/
//输入：7 11 1 2 3 4 5 6 7
//  1,2,19, 2,3,5, 1,7,18, 1,5,14, 2,5,12, 2,4,7, 3,4,3, 7,5,16, 5,4,8, 4,6,21,7,6,27,
//for(i=0;i<g->edgenum;i++)
//printf("%d,%d,%d\n",g->edgelist[i].ivex,g->edgelist[i].jvex,g->edgelist[i].weight);
}

void BubbleSort(Enode a[], int n)
//将a中整数序列重新排列成自小至大有序的整数序列
{int i,j,change=0;
Enode y;
for (i=n-1, change=1; i>=1 && change; --i)
for (j=0, change=0; j<i; ++j)
if (a[j].weight>a[j+1].weight)
    {
     //以y作中介，交换a[j],a[j+1]
    y=a[j];
    a[j]=a[j+1];
    a[j+1]=y;
    change= 1;
    }
}

MSTEdge *Kruskal(EGraph *g){
int i,j,k,s1,s2;
MSTEdge *TE; TE=(MSTEdge *)malloc(g->vexnum*sizeof(MSTEdge));
int *vSet; vSet=(int *)malloc(g->vexnum*sizeof(int));
for(i=0;i<g->vexnum;i++) vSet[i]=i;//初始化数组vSet
BubbleSort(g->edgelist,g->edgenum);//对边按权值从小到大排序
i=0;j=0;
while(i<g->vexnum && j<g->edgenum) {
    s1=vSet[g->edgelist[j].ivex];
    s2=vSet[g->edgelist[j].jvex];
    //若边的两个顶点的连通分量编号不同, 则将该边加入到TE中
    if(s1!=s2) {
        TE[i].vex1=g->edgelist[j].ivex;
        TE[i].vex2=g->edgelist[j].jvex;
        TE[i].weight=g->edgelist[j].weight;
        i++;
        for(k=0;k<g->vexnum;k++)
            if(vSet[k]==s2) vSet[k]=s1;
    }
    j++;
}
free(vSet);
return TE;
}

int main()
{
EGraph g;
MSTEdge *edges;
CreateGraph (&g);
edges=Kruskal(&g);
printf("MST:\n");
for(int i=0;i<g.vexnum-1;i++)
    printf("%d %d %d\n",edges[i].vex1,edges[i].vex2,edges[i].weight);
system("pause");
return 0;
}
