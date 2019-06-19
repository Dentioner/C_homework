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
int exist[MAX_VERTEX_NUM] = { 0 };//�����еĵ㲻����
int vi, vj;
int visited[MAX_VERTEX_NUM] = { 0 };
int findit = 0;
int k;

//��BFS
void create_graph()//ע�����������ͼ
{
	int ch;
	int v1, v2;
	int v3, v4;//�Ը�����ͼ��ʱ��ߵ�һ��v2v1�õ�v3v4
	int index1, index2;
	ArcNode *p, *q, *r, *p2, *q2, *r2;
	scanf("%d,%d", &(g.vexnum), &k);
	ch = getchar();//  \n
	scanf("%d,%d", &vi, &vj);
	ch = getchar();// \n

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
		v3 = v2;
		v4 = v1;
		p = (ArcNode*)malloc(sizeof(ArcNode));
		p2 = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = v2;
		p2->adjvex = v4;
		p->nextarc = NULL;
		p2->nextarc = NULL;
		index1 = 0;
		index2 = 0;
		while (g.vertices[index1].data != v1)
		{
			index1++;
		}//��һ��v1����
		while (g.vertices[index2].data != v3)
		{
			index2++;
		}//��һ��v3����
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
		//����һ���������䣬��v3v4���ȥ
		if (g.vertices[index2].firstarc == NULL)
			g.vertices[index2].firstarc = p2;//һ���ڵ㶼û�е�ʱ��
		else
		{
			//index2 = 0;
			q2 = g.vertices[index2].firstarc;
			while (p2->adjvex < q2->adjvex)
			{
				q2 = q2->nextarc;
				if (q2 == NULL)
					break;
			}
			//�嵽q��ǰ��
			r2 = g.vertices[index2].firstarc;
			if (r2 == q2)//p�����еĶ���
			{
				g.vertices[index2].firstarc = p2;
				p2->nextarc = r2;
			}
			else
			{
				while (r2->nextarc != q2 && r2->nextarc != NULL)
					r2 = r2->nextarc;
				r2->nextarc = p2;
				p2->nextarc = q2;
			}
		}

		scanf("%d-%d", &v1, &v2);
		ch = getchar();//',' or '\n'
	}

	exist[v1] = 1;
	exist[v2] = 1;
	v3 = v2;
	v4 = v1;
	p = (ArcNode*)malloc(sizeof(ArcNode));
	p->adjvex = v2;
	p->nextarc = NULL;
	p2 = (ArcNode*)malloc(sizeof(ArcNode));
	p2->adjvex = v4;
	p2->nextarc = NULL;
	index1 = 0;
	index2 = 0;
	while (g.vertices[index1].data != v1)
	{
		index1++;
	}//��һ��v1����
	while (g.vertices[index2].data != v3)
	{
		index2++;
	}//��һ��v3����
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

	if (g.vertices[index2].firstarc == NULL)
		g.vertices[index2].firstarc = p2;
	else
	{
		//index2 = 0;
		q2 = g.vertices[index2].firstarc;
		while (p2->adjvex < q2->adjvex)
		{
			q2 = q2->nextarc;
			if (q2 == NULL)
				break;
		}
		//�嵽q��ǰ��
		r2 = g.vertices[index2].firstarc;
		if (r2 == q2)//p�����еĶ���
		{
			g.vertices[index2].firstarc = p2;
			p2->nextarc = r2;
		}
		else
		{
			while (r2->nextarc != q2 && r2->nextarc != NULL)
				r2 = r2->nextarc;
			r2->nextarc = p2;
			p2->nextarc = q2;
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

int main()
{
	create_graph();
	print_graph();
	return 0;
}
