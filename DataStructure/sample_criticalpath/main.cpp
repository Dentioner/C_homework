#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AGraph.h"
using namespace std;

Status CriticalPath(AGraph *g) {
char orderedV[MAX_VERTEX_NUM];
int top;
//ve��vl�ֱ𱣴�������¼������緢��ʱ�����ٷ���ʱ��
int ve[MAX_VERTEX_NUM],vl[MAX_VERTEX_NUM];
int i,j,k,el,ee,dut; 
char tag; 
NodeLink *p;

if (!TopologicalOrder(g, orderedV, ve))
    return ERROR;
/*
printf("\nResults of Topological Sort: ");
for(i=0;i<g->vexnum;i++) printf("%c ",orderedV[i]);
printf("\n����/�¼������緢��ʱ��Array ve: ");
for(i=0;i<g->vexnum;i++) printf("%d ",ve[i]);
printf("\n");
*/
//��ʼ�������¼�����ٷ���ʱ��
for(i=0;i<g->vexnum;i++)
    vl[i] = ve[g->vexnum-1];
// ������������������vlֵ
top=g->vexnum;
while (top)
	for(top--,j=orderedV[top]-'A',p=g->v[j].first;p; p=p->next) {
        k=p->vindex; 
        dut=p->info; //dut<j,k>
        if (vl[j]>vl[k]-dut)
            vl[j]=vl[k]-dut;
    }

//for(i=0;i<g->vexnum;i++) printf("%d ",vl[i]);

printf("\nCritical Activities:\n");
for (j=0; j<g->vexnum; ++j) // ��ee, el�͹ؼ��
    for (p=g->v[j].first; p; p=p->next){
        k=p->vindex; dut=p->info;
        ee = ve[j]; 
        el = vl[k]-dut;
        tag = (ee==el) ? '*' : ' ';
        if(tag=='*') printf("%c %c \n",'A'+j, 'A'+k);
        // ����ؼ��
        //printf("%d %d %d %d %d %c\n",j, k, dut, ee, el, tag);
    }
return OK;
} // CriticalPath


int main()
{
AGraph g;
CreateDirectedGraph(&g);
ListGraph(&g);
if(CriticalPath(&g)) printf("\n ");
else printf("\n The graph has a circle.\n");
system("pause");
return 0;

}
