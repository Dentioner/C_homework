#ifndef EGRAPH_H_INCLUDED
#define EGRAPH_H_INCLUDED

#define MAX_VERTEX_NUM 30
#define MAX_EDGE_NUM 30
typedef int ElemType;
typedef struct Enode {
    //�����������������㼰Ȩ��
    int ivex,jvex,weight;
}Enode;

typedef struct {
    int vexnum,edgenum;
    ElemType vexlist[MAX_VERTEX_NUM];
    Enode edgelist[MAX_EDGE_NUM];
}EGraph;

typedef struct MSTEdge{
    //�����������������㼰Ȩ��
    int vex1,vex2,weight;
}MSTEdge;
#endif // EGRAPH_H_INCLUDED
