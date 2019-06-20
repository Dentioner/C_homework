#pragma once
#ifndef MGRAPH_H_INCLUDED
#define MGRAPH_H_INCLUDED

#define INFINITY 1000
#define MAX_VERTEX_NUM 30
typedef struct {
	int vexnum, arcnum;  //顶点数，边数
	char vexs[MAX_VERTEX_NUM]; //存放顶点信息
	int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; //存放边的信息
}MGraph;
void CreateGraph(MGraph *g);
void ListGraph(MGraph *g);

#endif // MGRAPH_H_INCLUDED
