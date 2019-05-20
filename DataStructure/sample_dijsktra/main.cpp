#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "MGraph.h"
using namespace std;

//��������G��v0���㵽���ඥ��v�����·��P[v]�����Ȩ����dist[v]
//P�ĵ�i�з���Ԫ�ر�ʾ��v0��vi�����·���ϵ�Ԫ��
//��P[v][w]ΪTRUE����w�Ǵ�v0��v��ǰ������·���ϵĶ���
void ShortestPath_DIJ(MGraph *g,int v0,int P[6][6], int dist[])
{
	int i=0,j, v,w,min;
	int final[MAX_VERTEX_NUM];
//����final[n]���ڱ�ʶһ�������Ƿ��Ѽ���S�С�final[v]Ϊ1���ҽ���v��S�������Ѿ���ô�v0��v�����·��
//P�ĵ�i�з���Ԫ�ر�ʾ��v0��vi�����·���ϵ�
//dist[]�����˵�ǰ����õĴ�Դ�㵽�������������·��ֵ

	for (v=0; v<g->vexnum; v++)
	{
		final[v]=0;
		dist[v]=g->arcs[v0][v];
		for (w=0; w<g->vexnum; w++)
			P[v][w] = 0; // ���·��
		if (dist[v] < INFINITY) 
		{
			P[v][v0] = 1; 
			P[v][v] = 1; 
		}
	}
	dist[v0] = 0; 
	final[v0] = 1; // ��ʼ����v0��������S��
	//��ѭ����ÿ�����v0��ĳ��v��������·������v��S��
	for (i=1; i<g->vexnum; i++) 
	{ //����g->vexnum-1������
		min = INFINITY; //��ǰ��֪��v0������������
		printf("Current dist:");
		for (w=0; w<g->vexnum; ++w)//�������ж���
			printf("%d ",dist[w]);//��ӡdist����
		printf("\n");
		for (w=0; w<g->vexnum; ++w)
			if (!final[w]) // ���w������V-S��
				if (dist[w]<min) // ���w������v0�������
					{ 
						v = w; 
						min = dist[w]; 
					}
		final[v] = 1; // ��v0���������v����S��
		// ���µ�ǰ���·��������
		for (w=0; w<g->vexnum; w++)
			if (!final[w]&&(min+g->arcs[v][w]<dist[w])) 
			{
				// �޸�dist[w]��P[w], w��V-S
				dist[w] = min + g->arcs[v][w];
				for(j=0;j<g->vexnum;j++)
					P[w][j] = P[v][j];
					//��v�и�ֵ�ڵ�w��
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
