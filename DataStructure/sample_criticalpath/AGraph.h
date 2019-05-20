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
    int vindex; //邻接点在头结点数组中的位置(下标)
    int info;   //(vertex,vindex) 的权重
    struct node *next;//指向下一个表结点
} NodeLink; // 表结点类型定义
typedef struct {
    // 图的顶点数、边数、种类标志
    int vexnum,edgenum;
    struct {
        ElemType vertex; //表的顶点用大写字母表示
        NodeLink *first; // 指向第一个表结点
    }v[MAX_VERTEX_NUM];
}AGraph;

void CreateDirectedGraph(AGraph *g);
void ListGraph(AGraph *g);
Status TopologicalOrder(AGraph *g, char stack[],int ve[]);

#endif // AGRAPH_H_INCLUDED
