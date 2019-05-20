#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "MGraph.h"
using namespace std;

//求有向网G的v0顶点到其余顶点v的最短路径P[v]及其带权长度dist[v]
//P的第i行非零元素表示从v0到vi的最短路径上的元素
//若P[v][w]为TRUE，则w是从v0到v当前求得最短路径上的顶点
void ShortestPath_DIJ(MGraph *g,int v0,int P[6][6], int dist[])
{
	int i=0,j, v,w,min;
	int final[MAX_VERTEX_NUM];
//数组final[n]用于标识一个顶点是否已加入S中。final[v]为1当且仅当v∈S，即：已经求得从v0到v的最短路径
//P的第i行非零元素表示从v0到vi的最短路径上的
//dist[]保存了当前已求得的从源点到其余各顶点的最短路径值

	for (v=0; v<g->vexnum; v++)
	{
		final[v]=0;
		dist[v]=g->arcs[v0][v];
		for (w=0; w<g->vexnum; w++)
			P[v][w] = 0; // 设空路径
		if (dist[v] < INFINITY) 
		{
			P[v][v0] = 1; 
			P[v][v] = 1; 
		}
	}
	dist[v0] = 0; 
	final[v0] = 1; // 初始化，v0顶点属于S集
	//主循环：每次求得v0到某个v顶点的最短路径并加v到S集
	for (i=1; i<g->vexnum; i++) 
	{ //其余g->vexnum-1个顶点
		min = INFINITY; //当前所知离v0顶点的最近距离
		printf("Current dist:");
		for (w=0; w<g->vexnum; ++w)//遍历所有顶点
			printf("%d ",dist[w]);//打印dist数组
		printf("\n");
		for (w=0; w<g->vexnum; ++w)
			if (!final[w]) // 如果w顶点在V-S中
				if (dist[w]<min) // 如果w顶点离v0顶点更近
					{ 
						v = w; 
						min = dist[w]; 
					}
		final[v] = 1; // 离v0顶点最近的v加入S集
		// 更新当前最短路径及距离
		for (w=0; w<g->vexnum; w++)
			if (!final[w]&&(min+g->arcs[v][w]<dist[w])) 
			{
				// 修改dist[w]和P[w], w∈V-S
				dist[w] = min + g->arcs[v][w];
				for(j=0;j<g->vexnum;j++)
					P[w][j] = P[v][j];
					//第v行赋值于第w行
				P[w][w] = 1; // P[w] = P[v]+[w]
			}//if
	}//for
	for(i=0;i<g->vexnum;i++) 
	{
		for(j=0;j<g->vexnum;j++)
			printf("%d ",P[i][j]);
		printf("\n");
	}
	for(i=0;i<g->vexnum;i++)
		printf("%d ",dist[i]);
} //ShortestPath_DIJ



int main()
{
	MGraph g;
	CreateGraph(&g);
	ListGraph(&g);

	int P[6][6];
	int D[6];
	ShortestPath_DIJ(&g,0,P,D);
	system("pause");
	return 0;
}
