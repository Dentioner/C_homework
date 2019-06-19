#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 20

typedef struct ArcNode
{
	int adjvex;//�û���ָ��Ķ����λ��
	struct ArcNode *nextarc;//ָ����һ������ָ��
	int info;//�û��������Ϣ
}ArcNode;

typedef struct VNode
{
	int data;//������Ϣ
	ArcNode *firstarc;//ָ���һ�������ö���Ļ���ָ��
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct
{
	AdjList vertices;
	int vexnum, arcnum;//������������
	int kind;//ͼ������
}ALGraph;

ALGraph g;

void create_graph()
{
	int ch;
	int v1, v2;
	int index1, index2;
	ArcNode *p, *q;
	scanf("%d,%d", &(g.vexnum), &(g.arcnum));
	ch = getchar();//  \n

	for (int i = 0; i < g.vexnum; i++)//��ʼ���ڽӱ�
	{
		g.vertices[i].data = i;
		g.vertices[i].firstarc = NULL;
	}

	scanf("%d-%d", &v1, &v2);
	ch = getchar();//',' or '\n'
	while (ch != '\n' && ch != EOF && ch != '\0')
	{
		p = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = v2;
		p->nextarc = NULL;
		index1 = 0;
		while (g.vertices[index1].data != v1)
		{
			index1++;
		}//��һ��v1����
		if (g.vertices[index1].firstarc == NULL)
			g.vertices[index1].firstarc = p;
		else
		{
			//index2 = 0;
			q = g.vertices[index1].firstarc;
			while (q->nextarc != NULL)
			{
				q = q->nextarc;
			}
			q->nextarc = p;
			p->nextarc = NULL;
		}
		scanf("%d-%d", &v1, &v2);
		ch = getchar();//',' or '\n'
	}
	
	p = (ArcNode*)malloc(sizeof(ArcNode));
	p->adjvex = v2;
	p->nextarc = NULL;
	index1 = 0;
	while (g.vertices[index1].data != v1)
	{
		index1++;
	}//��һ��v1����
	if (g.vertices[index1].firstarc == NULL)
		g.vertices[index1].firstarc = p;
	else
	{
		//index2 = 0;
		q = g.vertices[index1].firstarc;
		while (q->nextarc != NULL)
		{
			q = q->nextarc;
		}
		q->nextarc = p;
		p->nextarc = NULL;
	}

	return;
}

int main()
{
	create_graph();
	return;
}
