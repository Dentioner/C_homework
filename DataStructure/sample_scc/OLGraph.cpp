#include "OLGraph.h"

void CreateGraph(OLGraph *g) { // G.kind = DG
int i,j,k; char v1,v2;
struct ArcBox *p;

printf("Input vexnum, arcnum ");
scanf("%d %d",&g->vexnum, &g->arcnum);
for(i=0; i<g->vexnum; ++i) { //构造表头向量
        g->xlist[i].data='A'+i; //设置顶点值
        g->xlist[i].firstin = g->xlist[i].firstout = NULL;
        }
for(k=0; k<g->arcnum; ++k) { //输入各弧并构造十字链表
        scanf(" %c%c",&v1, &v2); //输入一条弧的始点和终点
        i=LocateVex(g,v1);
        j=LocateVex(g,v2); //确定v1和v2在g中位置
        p=(ArcBox *) malloc (sizeof (ArcBox)); //假定有足够空间
        p->tailvex=i; p->headvex=j;
        p->hlink=g->xlist[j].firstin;
        p->tlink=g->xlist[j].firstout;
        g->xlist[j].firstin = g->xlist[i].firstout = p; //在入弧和出弧链头插入
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
void DFS(OLGraph *g,int x){//按弧的正向搜索
ArcBox *p ;
visited[x]=1;
for(p=g->xlist[x].firstout; p!=NULL ; p=p->tlink)
	if (!visited[p->headvex])
        DFS(g,p->headvex);
finished[count++]=x;
}
void  ReverseDFS(OLGraph *g,int x) {//对图G按弧的逆向进行搜索
ArcBox *p;
visited[x]=1;
printf("%d",x);//输出顶点
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
if (!visited[i])  DFS(g,i); //对图G正向遍历

for(i=0;i<g->vexnum;i++)
    printf("%d %d\n",i,finished[i]);

for(i=0; i<g->vexnum; i++)
visited[i]=0;
for(j=g->vexnum-1;j>=0;j--) {
    v=finished[j];
    if (!visited[v]) {
        printf("\n第%d个连通分量顶点:", k++);
        ReverseDFS(g,v); //对图G逆向遍历
        printf("\n");
        }
    }
}


