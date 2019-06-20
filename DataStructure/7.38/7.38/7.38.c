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
int print_array[MAX_VERTEX_NUM] = { 0 };
int length_of_print_array = 0;

int compare(int adj1, int adj2)//比较两个adj的“大小”
{//实际上就是在比较优先度
//遍历的时候是字母比算符优先
//别的暂时不知道
//adj1<adj2的时候输出1
	if ((adj1 == '+' || adj1 == '-' || adj1 == '*' || adj1 == '/') &&
		(adj2 != '+' && adj2 != '-' && adj2 != '*' && adj2 != '/'))
		return 1;
	else if (adj2 > adj1)
		return 1;
	else
		return 0;
}

void create_graph()
{
	int ch;

	//int v1, v2;
	int index1, index2;
	ArcNode *p, *q, *r;
	scanf("%d", &(g.vexnum));
	ch = getchar();//  \n
	for (index1 = 0; index1 < g.vexnum; index1++)
	{
		ch = getchar();//每一行开头的那个字符
		g.vertices[index1].data = ch;
		exist[index1] = 1;
		ch = getchar();//检索下一个字符，可能是空格或者换行
		while (ch != '\n')//如果不是换行，ch就是空格
		{
			scanf("%d", &index2);//这时候再读一个字符，就是下一个节点的索引了，这个一定是数字，就用scanf读
			p = (ArcNode*)malloc(sizeof(ArcNode));
			p->adjvex = index2;//注意，这一轮下来，链表头结点还没有读取完毕，先将index2的值存在这里，等全部读完了再改
			p->nextarc = NULL;
			if (g.vertices[index1].firstarc == NULL)
				g.vertices[index1].firstarc = p;//一个节点都没有的时候
			else
			{ 
				g.vertices[index1].firstarc->nextarc = p;
			}

			ch = getchar();//读取下一个字符，可能是空格或者换行
		}
	}
	return;
}

void rearrange_graph()//将索引值改成真实的节点值
//搞错了，不是真实节点值，还是原来的索引值好一些
{
	int index1;
	ArcNode *p, *q;
	int p_adj, q_adj;
	for (index1 = 0; index1 < g.vexnum; index1++)
	{
		if (g.vertices[index1].firstarc == NULL)
			continue;
		p = g.vertices[index1].firstarc;
		q = p->nextarc;
		p_adj = g.vertices[p->adjvex].data;
		q_adj = g.vertices[q->adjvex].data;
		if (compare(p_adj, q_adj))//如果p是算符，而q是字母的话，需要交换pq的位置
		{
			g.vertices[index1].firstarc = q;
			q->nextarc = p;
			p->nextarc = NULL;//因为每个节点就两个儿子
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
		printf("%c", g.vertices[index1].data);
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

void r_DFS(int v)
{
	ArcNode * w;
	visited[v] = 1;
	print_array[length_of_print_array++] = g.vertices[v].data;

	
	for (w = g.vertices[v].firstarc; w != NULL; w = w->nextarc)
	{
		if (!visited[w->adjvex])
			r_DFS(w->adjvex);
	}

	visited[v] = 0;
	return;
}

void reverse_DFStraverse()//反向深度遍历
{
	r_DFS(0);
	for (int i = length_of_print_array - 1; i >= 0; i--)
		printf("%c", print_array[i]);
	return;
}

int main()
{
	create_graph();
	//print_graph();
	//rearrange_graph();
	//print_graph();
	reverse_DFStraverse();
	return 0;
}