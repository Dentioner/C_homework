#pragma once
#ifndef MGRAPH_H_INCLUDED
#define MGRAPH_H_INCLUDED

#define INFINITY 1000
#define MAX_VERTEX_NUM 30
typedef struct {
	int vexnum, arcnum;  //������������
	char vexs[MAX_VERTEX_NUM]; //��Ŷ�����Ϣ
	int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; //��űߵ���Ϣ
}MGraph;
void CreateGraph(MGraph *g);
void ListGraph(MGraph *g);

#endif // MGRAPH_H_INCLUDED
