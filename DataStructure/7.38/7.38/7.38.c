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
int print_array[MAX_VERTEX_NUM] = { 0 };
int length_of_print_array = 0;

int compare(int adj1, int adj2)//�Ƚ�����adj�ġ���С��
{//ʵ���Ͼ����ڱȽ����ȶ�
//������ʱ������ĸ���������
//�����ʱ��֪��
//adj1<adj2��ʱ�����1
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
		ch = getchar();//ÿһ�п�ͷ���Ǹ��ַ�
		g.vertices[index1].data = ch;
		exist[index1] = 1;
		ch = getchar();//������һ���ַ��������ǿո���߻���
		while (ch != '\n')//������ǻ��У�ch���ǿո�
		{
			scanf("%d", &index2);//��ʱ���ٶ�һ���ַ���������һ���ڵ�������ˣ����һ�������֣�����scanf��
			p = (ArcNode*)malloc(sizeof(ArcNode));
			p->adjvex = index2;//ע�⣬��һ������������ͷ��㻹û�ж�ȡ��ϣ��Ƚ�index2��ֵ���������ȫ���������ٸ�
			p->nextarc = NULL;
			if (g.vertices[index1].firstarc == NULL)
				g.vertices[index1].firstarc = p;//һ���ڵ㶼û�е�ʱ��
			else
			{ 
				g.vertices[index1].firstarc->nextarc = p;
			}

			ch = getchar();//��ȡ��һ���ַ��������ǿո���߻���
		}
	}
	return;
}

void rearrange_graph()//������ֵ�ĳ���ʵ�Ľڵ�ֵ
//����ˣ�������ʵ�ڵ�ֵ������ԭ��������ֵ��һЩ
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
		if (compare(p_adj, q_adj))//���p���������q����ĸ�Ļ�����Ҫ����pq��λ��
		{
			g.vertices[index1].firstarc = q;
			q->nextarc = p;
			p->nextarc = NULL;//��Ϊÿ���ڵ����������
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

void reverse_DFStraverse()//������ȱ���
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