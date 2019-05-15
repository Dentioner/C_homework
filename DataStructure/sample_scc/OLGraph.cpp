#include "OLGraph.h"

void CreateGraph(OLGraph *g) { // G.kind = DG
int i,j,k; char v1,v2;
struct ArcBox *p;

printf("Input vexnum, arcnum ");
scanf("%d %d",&g->vexnum, &g->arcnum);
for(i=0; i<g->vexnum; ++i) { //�����ͷ����
        g->xlist[i].data='A'+i; //���ö���ֵ
        g->xlist[i].firstin = g->xlist[i].firstout = NULL;
        }
for(k=0; k<g->arcnum; ++k) { //�������������ʮ������
        scanf(" %c%c",&v1, &v2); //����һ������ʼ����յ�
        i=LocateVex(g,v1);
        j=LocateVex(g,v2); //ȷ��v1��v2��g��λ��
        p=(ArcBox *) malloc (sizeof (ArcBox)); //�ٶ����㹻�ռ�
        p->tailvex=i; p->headvex=j;
        p->hlink=g->xlist[j].firstin;
        p->tlink=g->xlist[j].firstout;
        g->xlist[j].firstin = g->xlist[i].firstout = p; //���뻡�ͳ�����ͷ����
        }
return;
}

int LocateVex(OLGraph *g,ElemType v){
int k;
for(k=0;k<g->vexnum;k++)
    if (g->xlist[k].data == v) return k;
return -1;
}

int visited[MAX_VERTEX_NUM];
int finished[MAX_VERTEX_NUM],count;
void DFS(OLGraph *g,int x){//��������������
ArcBox *p ;
visited[x]=1;
for(p=g->xlist[x].firstout; p!=NULL ; p=p->tlink)
	if (!visited[p->headvex])
        DFS(g,p->headvex);
finished[count++]=x;
}
void  ReverseDFS(OLGraph *g,int x) {//��ͼG�����������������
ArcBox *p;
visited[x]=1;
printf("%d",x);//�������
for(p=g->xlist[x].firstin; p!=NULL;p=p->hlink)
	if(!visited[p->tailvex])
	ReverseDFS(g,p->tailvex);
}

void ConnectedComponent(OLGraph *g)
{int i,j,v,k=1;
count=0;
for(i=0; i<g->vexnum; i++)
visited[i]=0;
for(i=0; i<g->vexnum; i++)
if (!visited[i])  DFS(g,i); //��ͼG�������

for(i=0;i<g->vexnum;i++)
    printf("%d %d\n",i,finished[i]);

for(i=0; i<g->vexnum; i++)
visited[i]=0;
for(j=g->vexnum-1;j>=0;j--) {
    v=finished[j];
    if (!visited[v]) {
        printf("\n��%d����ͨ��������:", k++);
        ReverseDFS(g,v); //��ͼG�������
        printf("\n");
        }
    }
}


