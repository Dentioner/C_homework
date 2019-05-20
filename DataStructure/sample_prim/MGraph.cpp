#include "MGraph.h"

//Undirected graph:kind=0 Directed graph:kind=1
void CreateGraph(MGraph *g) {
int i,j,k,weight;
printf("Input: Vex Num & Arc Num & Arc Kind\n");
scanf("%d %d %d",&g->vexnum,&g->arcnum,&k);
switch (k) {
case 0:
    g->kind=UDG;break;
case 1:
    g->kind=DG;break;
}

for(k=0;k<g->vexnum;k++)
    g->vexs[k]='A'+k;
for(i=0;i<g->vexnum;i++)
    for(j=0;j<g->vexnum;j++)
    g->A[i][j] = MAX_WEIGHT;//��Ȩ�صĳ�ʼֵ
printf("Input: %d edges: \n",g->arcnum);
for(k=0;k<g->arcnum;k++){
    scanf("%d %d %d",&i,&j,&weight);
    g->A[i][j]=weight;
    if(g->kind==UDG)
        g->A[j][i]=weight;
}
}

void ListGraph(MGraph *g){
int i,j;
printf("ListGraph:\n");
for(i=0;i<g->vexnum;i++) {
    printf("%6c---",g->vexs[i]);
    for(j=0;j<g->vexnum;j++)
        printf("%7d",g->A[i][j]);
    printf("\n");
    }
}

int LocateVex(MGraph *g,ElemType e){
int i;
for(i=0;i<g->vexnum;i++)
    if(g->vexs[i]==e) return i;
return MAX_VERTEX_NUM;
}

//MSTEdge *MSTByPrim(MGraph *g,ElemType u){
void MSTByPrim(MGraph *g,ElemType u){
int i,j,k;
struct {
    ElemType  adjvex;	//����������U�еĶ���
    int  lowcost; 	    //�ñߵ�Ȩֵ
} closedge[MAX_VERTEX_NUM];

k=LocateVex(g,u);
//closedge[k].adjvex=u;
closedge[k].lowcost=0; //��ʼʱ��U��{u}
for (j=0; j<g->vexnum; ++j ) { //���������ʼ��
	if (j!=k) {
            closedge[j].adjvex=u;
            closedge[j].lowcost=g->A[k][j];
            }
}
//for(j=0;j<g->vexnum;j++) printf("closedge[%d]:%c,%d\n",j,closedge[j].adjvex,closedge[j].lowcost);
for (i=1; i<g->vexnum; ++i) {
	//ѡ������g->vexnum-1��������ӵ���������
	//�������������һ����㣺��k����
    //k = minimum(closedge);
    k=0; int x,min=MAX_WEIGHT;
    for(x=0;x<g->vexnum;x++) {
        if(closedge[x].lowcost!=0 && min>closedge[x].lowcost){
            min=closedge[x].lowcost;k=x;
            }
    }
    //����������ı�
    printf("������=%c ��������=%c weight=%d \n",closedge[k].adjvex,g->vexs[k],closedge[k].lowcost);
    closedge[k].lowcost = 0; //��k���㲢��U��
    for (j=0; j<g->vexnum; ++j)
        if (g->A[j][k] < closedge[j].lowcost) {
        //�޸�V-U�����������������С��
        closedge[j].adjvex=g->vexs[k];
        closedge[j].lowcost=g->A[j][k];
        }
    //for(j=0;j<g->vexnum;j++) printf("closedge[%d]:%c,%d\n",j,closedge[j].adjvex,closedge[j].lowcost);
    }
} //MSTByPrim
