#ifndef OLGRAPH_H_INCLUDED
#define OLGRAPH_H_INCLUDED
#include <stdio.h>
#include <malloc.h>

#define MAX_VERTEX_NUM  30 //最大顶点数
typedef char ElemType;
typedef struct ArcBox {
    int  tailvex, headvex;//尾结点和头结点在图中的位置
    //InfoType    info;   // 与弧相关的信息 如权值
    struct ArcBox *hlink,
                  *tlink; //分别链接弧头相同和弧尾相同的弧
}ArcNode; //弧结点
typedef struct VexNode {
    ElemType  data;    // 顶点信息
    ArcBox  *firstin,
            *firstout; //分别指向该顶点第一条入弧和出弧
}VexNode;//顶点结点
typedef struct {
    int vexnum, arcnum;
    VexNode  xlist[MAX_VERTEX_NUM];
}OLGraph;

void CreateGraph(OLGraph *g);
int LocateVex(OLGraph *g,ElemType v);
void ConnectedComponent(OLGraph *g);

#endif // OLGRAPH_H_INCLUDED
