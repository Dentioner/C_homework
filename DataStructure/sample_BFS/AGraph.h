#ifndef AGRAPH_H_INCLUDED
#define AGRAPH_H_INCLUDED

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTEX_NUM 30
#define UDG 0
#define DG 1

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
int FirstAdjVex(AGraph *g,int x); 	 //���ض���x�ĵ�һ���ڽӶ���
int NextAdjVex(AGraph *g,int x, int y); //���ض���x��(�����y��)��һ���ڽӶ���
void BFS(AGraph *g);

#endif // AGRAPH_H_INCLUDED
