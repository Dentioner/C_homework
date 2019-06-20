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
	int MPL;
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct
{
	AdjList vertices;
	int vexnum, arcnum;//������������
	int kind;//ͼ������
}ALGraph;

ALGraph g;
int exist[MAX_VERTEX_NUM] = { 0 };//�����еĵ㲻����
int vi, vj;
int visited[MAX_VERTEX_NUM] = { 0 };
int findit = 0;
int start_point = 0;

void create_graph()
{
	int ch;
	int v1, v2;
	int index1, index2;
	ArcNode *p, *q, *r;
	scanf("%d", &(g.vexnum));
	ch = getchar();//  \n

	for (int i = 0; i < MAX_VERTEX_NUM; i++)//��ʼ���ڽӱ�
	{
		g.vertices[i].data = i;
		g.vertices[i].firstarc = NULL;
	}

	scanf("%d-%d", &v1, &v2);
	ch = getchar();//',' or '\n'
	while (ch != '\n' && ch != EOF && ch != '\0')
	{
		exist[v1] = 1;
		exist[v2] = 1;
		p = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = v2;
		p->nextarc = NULL;
		index1 = 0;
		while (g.vertices[index1].data != v1)
		{
			index1++;
		}//��һ��v1����
		if (g.vertices[index1].firstarc == NULL)
			g.vertices[index1].firstarc = p;//һ���ڵ㶼û�е�ʱ��
		else
		{
			//index2 = 0;
			q = g.vertices[index1].firstarc;
			while (p->adjvex < q->adjvex)
			{
				q = q->nextarc;
				if (q == NULL)
					break;
			}
			//�嵽q��ǰ��
			r = g.vertices[index1].firstarc;
			if (r == q)//p�����еĶ���
			{
				g.vertices[index1].firstarc = p;
				p->nextarc = r;
			}
			else
			{
				while (r->nextarc != q && r->nextarc != NULL)
					r = r->nextarc;
				r->nextarc = p;
				p->nextarc = q;
			}
		}
		scanf("%d-%d", &v1, &v2);
		ch = getchar();//',' or '\n'
	}

	exist[v1] = 1;
	exist[v2] = 1;
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
		while (p->adjvex < q->adjvex)
		{
			q = q->nextarc;
			if (q == NULL)
				break;
		}
		//�嵽q��ǰ��
		r = g.vertices[index1].firstarc;
		if (r == q)//p�����еĶ���
		{
			g.vertices[index1].firstarc = p;
			p->nextarc = r;
		}
		else
		{
			while (r->nextarc != q && r->nextarc != NULL)
				r = r->nextarc;
			r->nextarc = p;
			p->nextarc = q;
		}
	}

	//scanf("%d,%d", &vi, &vj);

	return;
}


void print_graph()
{
	int index1;
	int print_counter = 0;
	ArcNode *p;
	for (index1 = 0; print_counter < g.vexnum; index1++)
	{
		if (!exist[index1])
			continue;
		p = g.vertices[index1].firstarc;
		printf("%d", g.vertices[index1].data);
		if (p != NULL)
			printf(" ");
		print_counter++;
		while (p != NULL)
		{
			printf("%d", p->adjvex);
			if (p->nextarc != NULL)
				printf(",");
			p = p->nextarc;
		}
		if (print_counter <= g.vexnum - 1)
			printf("\n");
	}
	return;
}

void DFS(int v, int depth)
{
	ArcNode * w;
	visited[v] = 1;
	if (v == vj)
		findit = 1;
	for (w = g.vertices[v].firstarc; w != NULL; w = w->nextarc)
	{
		if (!visited[w->adjvex])
			DFS(w->adjvex, depth + 1);
	}

	if (g.vertices[start_point].MPL < depth)
		g.vertices[start_point].MPL = depth;

	visited[v] = 0;
	return;
}

void DFSTraverse()
{
	int v;
	for (v = 0; v < g.vexnum; v++)
		visited[v] = 0;
	for (v = 0; v < MAX_VERTEX_NUM; v++)
	{
		if (!exist[v])
			continue;
		start_point = v;
		if (!visited[v])
			DFS(v, 0);
		for (int i = 1; i <= g.vexnum; i++)
			visited[i] = 0;
	}
	return;
}

void findMPL()
{
	int index;
	for (index = 0; index < MAX_VERTEX_NUM; index++)//��ʼ��
		g.vertices[index].MPL = 0;

	DFSTraverse();
	
}

void print_MPL()
{

	int index1;
	int print_counter = 0;
	ArcNode *p;
	for (index1 = 0; print_counter < g.vexnum; index1++)
	{
		if (!exist[index1])
			continue;
		p = g.vertices[index1].firstarc;
		printf("%d", g.vertices[index1].MPL);
		
		print_counter++;
		
		if (print_counter <= g.vexnum - 1)
			printf(",");
	}
	return;
}

int main()
{
	create_graph();
	//print_graph();
	findMPL();
	print_MPL();
	return 0;
}