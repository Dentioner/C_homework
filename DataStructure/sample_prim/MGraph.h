#ifndef MGRAPH_H_INCLUDED
#define MGRAPH_H_INCLUDED


#include <stdio.h>
#define MAX_VERTEX_NUM 20
#define MAX_WEIGHT 1000
typedef enum {UDG,DG} GraphKind;
typedef char ElemType;
typedef struct{
    int vexnum,arcnum;  //顶点数，边数
    GraphKind kind;     //图的种类
    ElemType vexs[MAX_VERTEX_NUM];     //存放顶点信息
    int A[MAX_VERTEX_NUM][MAX_VERTEX_NUM];    //存放边的信息
}MGraph;

void CreateGraph(MGraph *g);
void ListGraph(MGraph *g);
int LocateVex(MGraph *g,ElemType e);

typedef struct MSTEdge {
int  vex1, vex2; //边所依附的图中两个顶点
int  weight; //边的权值
} MSTEdge;

//MSTEdge *MSTByPrim(MGraph *g,ElemType e);
void MSTByPrim(MGraph *g,ElemType e);

#endif // MGRAPH_H_INCLUDED
