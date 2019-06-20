#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 10
#define INFINITY 1000
typedef struct ArcNode
{
	int adjvex;//�û���ָ��Ķ����λ��
	struct ArcNode *nextarc;//ָ����һ������ָ��
	int weight;//�û���Ȩֵ
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
int exist[MAX_VERTEX_NUM] = { 0 };//�����еĵ㲻����
int vi, vj;
int visited[MAX_VERTEX_NUM] = { 0 };
int findit = 0;
int start_point = 0;
int P[MAX_VERTEX_NUM][MAX_VERTEX_NUM] = { 0 };
int D[MAX_VERTEX_NUM] = { 0 };


void create_graph()
{
	int v1, v2, weight;
	int ch;
	int index1, index2;
	ArcNode *p, *q, *r;
	scanf("%d %d", &(g.vexnum), &start_point);
	ch = getchar();// '\n'

	for (int i = 0; i < MAX_VERTEX_NUM; i++)//��ʼ���ڽӱ�
	{
		g.vertices[i].data = i;
		g.vertices[i].firstarc = NULL;
	}

	scanf("%d-%d %d", &v1, &v2, &weight);
	ch = getchar();
	while (ch == '\n')
	{
		exist[v1] = 1;
		exist[v2] = 1;
		p = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = v2;
		p->nextarc = NULL;
		p->weight = weight;
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
		scanf("%d-%d %d", &v1, &v2, &weight);
		ch = getchar();
	}


	exist[v1] = 1;
	exist[v2] = 1;
	p = (ArcNode*)malloc(sizeof(ArcNode));
	p->adjvex = v2;
	p->nextarc = NULL;
	p->weight = weight;
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

void ShortestPath_DIJ()
{
	int i = 0, j, v, w, min;
	int index1;
	int final[MAX_VERTEX_NUM];//final[v] = 1��ʱ��˵���Ѿ���� ��v�����·����
	int weight1;
	ArcNode *p1, *p2;
	for (v = 0, p1 = g.vertices[start_point].firstarc; v < MAX_VERTEX_NUM && p1!= NULL; v++) 
	{
		final[v] = 0;
		if (!exist[v])
			continue;
		D[v] = p1->weight;
		for (w = 0; w < MAX_VERTEX_NUM; w++)
			P[v][w] = 0; // ���·��
		if (D[v] < INFINITY) 
		{
			P[v][start_point] = 1; 
			P[v][v] = 1;
		}
		p1 = p1->nextarc;
	}
	D[start_point] = 0; 
	final[start_point] = 1; // ��ʼ������ʼ������S��
   //��ѭ����ÿ�����start_point��ĳ��v��������·������v��S��
	for (i = 0; i < MAX_VERTEX_NUM; i++) 
	{ //����g->vexnum-1������
		if (i == start_point)
			continue;
		if (!exist[i])
			continue;
		min = INFINITY; //��ǰ��֪��v0������������
		//printf("Current dist:");
		//for (w = 0; w < g.vexnum; ++w)
			//printf("%d ", D[w]);
		//printf("\n");
		for (w = 0; w < g.vexnum; ++w)
			if (!final[w]) // w������V-S��
				if (D[w] < min) // w������v0�������
				{
					v = w; 
					min = D[w];
				}
		final[v] = 1; // ��start_point�����v����S��
		// ���µ�ǰ���·��������
		for (w = 0; w < MAX_VERTEX_NUM; w++)
		{
			if (!exist[w])
				continue;
			p2 = g.vertices[v].firstarc;
			if (p2 != NULL)
			{
				while (p2->adjvex != w)
				{
					p2 = p2->nextarc;//��λ arcs[v][w]
					if (p2 == NULL)
						break;
				}
			}
			if (p2 == NULL)
				weight1 = 0;
			else
				weight1 = p2->weight;
			if (!final[w] && (min + weight1 < D[w]))
			{
				// �޸�D[w]��P[w], w��V-S
				D[w] = min + weight1;
				for (j = 0; j < MAX_VERTEX_NUM; j++)
				{
					if (!exist[j])
						continue;
					P[w][j] = P[v][j];
				}//��v�и�ֵ�ڵ�w��
				P[w][w] = 1; // P[w] = P[v]+[w]
			}//if
		}
	}//for
	
}

int main()
{
	create_graph();
	print_graph();
	ShortestPath_DIJ();
	return 0;
}