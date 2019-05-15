#ifndef AGRAPH_H_INCLUDED
#define AGRAPH_H_INCLUDED
#include <stdio.h>
#include <malloc.h>

#define OK 1
#define ERROR 0
typedef int Status;

#define MAX_VERTEX_NUM 30
typedef char ElemType;
typedef struct node {
    int vindex; //�ڽӵ���ͷ��������е�λ��(�±�)
    struct node *next;//ָ����һ������
} NodeLink; // �������Ͷ���
typedef struct {
    // ͼ�Ķ������������������־
    int vexnum,edgenum,kind;
    struct {
        ElemType vertex;
        NodeLink *first;// ָ���һ������
    }v[MAX_VERTEX_NUM];
}AGraph;

void CreateGraph(AGraph *g,int n,int m);  //n��������mͼ������
void ListGraph(AGraph *g);
int LocateVex(AGraph *g,ElemType u);
ElemType GetVex(AGraph *g,int v);

#endif // AGRAPH_H_INCLUDED
