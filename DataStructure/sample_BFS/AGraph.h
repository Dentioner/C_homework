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
    int vindex; //邻接点在头结点数组中的位置(下标)
    struct node *next;//指向下一个表结点
} NodeLink; // 表结点类型定义
typedef struct {
    // 图的顶点数、边数、种类标志
    int vexnum,edgenum,kind;
    struct {
        ElemType vertex;
        NodeLink *first;// 指向第一个表结点
    }v[MAX_VERTEX_NUM];
}AGraph;

void CreateGraph(AGraph *g,int n,int m);  //n顶点数，m图的种类
void ListGraph(AGraph *g);
int LocateVex(AGraph *g,ElemType u);
int FirstAdjVex(AGraph *g,int x); 	 //返回顶点x的第一个邻接顶点
int NextAdjVex(AGraph *g,int x, int y); //返回顶点x的(相对于y的)下一个邻接顶点
void BFS(AGraph *g);

#endif // AGRAPH_H_INCLUDED
