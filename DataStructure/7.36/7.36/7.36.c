#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 20

typedef struct ArcNode
{
	int adjvex;//该弧所指向的顶点的位置
	struct ArcNode *nextarc;//指向下一条弧的指针
	int info;//该弧的相关信息
}ArcNode;

typedef struct VNode
{
	int data;//顶点信息
	ArcNode *firstarc;//指向第一条依附该顶点的弧的指针
	int MPL;
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct
{
	AdjList vertices;
	int vexnum, arcnum;//顶点数，边数
	int kind;//图的种类
}ALGraph;

ALGraph g;
int exist[MAX_VERTEX_NUM] = { 0 };//可能有的点不存在
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

	for (int i = 0; i < MAX_VERTEX_NUM; i++)//初始化邻接表
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
		}//找一下v1在哪
		if (g.vertices[index1].firstarc == NULL)
			g.vertices[index1].firstarc = p;//一个节点都没有的时候
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
			//插到q的前面
			r = g.vertices[index1].firstarc;
			if (r == q)//p比所有的都大
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
	}//找一下v1在哪
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
		//插到q的前面
		r = g.vertices[index1].firstarc;
		if (r == q)//p比所有的都大
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
	for (index = 0; index < MAX_VERTEX_NUM; index++)//初始化
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