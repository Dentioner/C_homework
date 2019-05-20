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
    int info;   //(vertex,vindex) ��Ȩ��
    struct node *next;//ָ����һ������
} NodeLink; // �������Ͷ���
typedef struct {
    // ͼ�Ķ������������������־
    int vexnum,edgenum;
    struct {
        ElemType vertex; //��Ķ����ô�д��ĸ��ʾ
        NodeLink *first; // ָ���һ������
    }v[MAX_VERTEX_NUM];
}AGraph;

void CreateDirectedGraph(AGraph *g);
void ListGraph(AGraph *g);
Status TopologicalOrder(AGraph *g, char stack[],int ve[]);

#endif // AGRAPH_H_INCLUDED
