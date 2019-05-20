#ifndef MGRAPH_H_INCLUDED
#define MGRAPH_H_INCLUDED


#include <stdio.h>
#define MAX_VERTEX_NUM 20
#define MAX_WEIGHT 1000
typedef enum {UDG,DG} GraphKind;
typedef char ElemType;
typedef struct{
    int vexnum,arcnum;  //������������
    GraphKind kind;     //ͼ������
    ElemType vexs[MAX_VERTEX_NUM];     //��Ŷ�����Ϣ
    int A[MAX_VERTEX_NUM][MAX_VERTEX_NUM];    //��űߵ���Ϣ
}MGraph;

void CreateGraph(MGraph *g);
void ListGraph(MGraph *g);
int LocateVex(MGraph *g,ElemType e);

typedef struct MSTEdge {
int  vex1, vex2; //����������ͼ����������
int  weight; //�ߵ�Ȩֵ
} MSTEdge;

//MSTEdge *MSTByPrim(MGraph *g,ElemType e);
void MSTByPrim(MGraph *g,ElemType e);

#endif // MGRAPH_H_INCLUDED
