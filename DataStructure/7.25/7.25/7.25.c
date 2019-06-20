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

int s[MAX_VERTEX_NUM] = { 0 };
int fst[MAX_VERTEX_NUM] = { 0 };
int lst[MAX_VERTEX_NUM] = { 0 };
//ע�⣺ͼ�Ķ����1��ʼ��s[0], fst[0]��lst[0]��û�����壬���ﶼ����Ϊ0
ALGraph g;
int exist[MAX_VERTEX_NUM] = { 0 };//�����еĵ㲻����
int vi, vj;
int visited[MAX_VERTEX_NUM] = { 0 };
int findit = 0;
int start_point = 0;

void create_array()
{
	int ch;
	int index;
	int number;
	scanf("%d", &(g.vexnum));
	ch = getchar();// '\n'
	//for (index = 0; index < g.vexnum + 1; index++)
	index = 0;
	do
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		s[index] = number;
		index++;
	} while (ch != '\n');
	s[index] = '\0';

	for (index = 0; index < g.vexnum + 1; index++)
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		fst[index] = number;
	}
	fst[g.vexnum + 1] = '\0';

	for (index = 0; index < g.vexnum + 1; index++)
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		lst[index] = number;
	}
	lst[g.vexnum + 1] = '\0';

	return;
}

void create_graph()
{
	int index1, index2;
	int sub;
	ArcNode *p, *q, *r;

	for (int i = 0; i < MAX_VERTEX_NUM; i++)//��ʼ���ڽӱ�
	{
		g.vertices[i].data = i;
		g.vertices[i].firstarc = NULL;
	}
	

	for (index1 = 1; index1 <= g.vexnum; index1++)
	{
		if (lst[index1] <= fst[index1])
			continue;
		for(index2 = fst[index1]; index2 < lst[index1]; index2++)
		{
			sub = s[index2];
			p = (ArcNode*)malloc(sizeof(ArcNode));
			p->adjvex = sub;
			//p->info = NULL;
			p->nextarc = NULL;
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
		}
	}

	return;
}

void print_graph()
{
	int index1;
	int print_counter = 0;
	ArcNode *p;
	for (index1 = 1; print_counter < g.vexnum; index1++)
	{
		//if (!exist[index1])
			//continue;
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

void DFS(int v)
{
	ArcNode * w;
	visited[v] = 1;
	
	for (w = g.vertices[v].firstarc; w != NULL; w = w->nextarc)
	{
		
		if (w->adjvex == start_point)
			findit = 1;
		if (!visited[w->adjvex])
			DFS(w->adjvex);
		//else
			//findit = 1;
		
	}

	return;
}

void DFSTraverse()
{
	int v;
	for (v = 0; v < g.vexnum; v++)
		//visited[v] = 0;
	for (v = 1; v <= g.vexnum; v++)
	{
		//if (!visited[v])
		start_point = v;
		DFS(v);
		for (int i = 1; i <= g.vexnum; i++)
			visited[i] = 0;
	}
	return;
}

int main()
{
	create_array();
	create_graph();
	//print_graph();
	DFSTraverse();
	if (findit)
		printf("yes");
	else
		printf("no");
	return 0;
}