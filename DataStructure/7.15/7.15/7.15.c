#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define INFINITY INT_MAX
#define MAX_VERTEX_NUM 20

typedef enum { DG, DN, UDG, UDN }GraphKind;
//DG：有向图；DN：有向网；UDG：无向图；UDN：无向网
typedef struct ArcCell
{
	int adj; //顶点
	int *info;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct
{
	int vexs[MAX_VERTEX_NUM];//顶点集
	AdjMatrix arcs;//邻接矩阵
	int vexnum, arcnum;//顶点数，弧数
	GraphKind kind;//图的种类
}MGraph;

typedef struct
{
	int iv;
	int ia1, ia2;
	int dv;
	int da1, da2;
}instruction;

MGraph g;
instruction inst;

int exist[MAX_VERTEX_NUM] = { 0 };//可能有的点不存在


void create_cg()
{
	int ch;
	int v1, v2;
	int index1, index2;
	scanf("%d", &(g.vexnum));
	ch = getchar();//'\n'

	for (int i = 0; i < g.vexnum; i++)//初始化
		for (int j = 0; j < g.vexnum; j++)
		{
			g.arcs[i][j].adj = INFINITY;
			g.arcs[i][j].info = NULL;
		}

	scanf("%d-%d", &v1, &v2);
	
	exist[v1] = 1;
	exist[v2] = 1;
	ch = getchar();//',' or '\n'

	while (ch != '\n' && ch != EOF && ch != '\0')
	{
		g.arcs[v1][v2].adj = 1;
		scanf("%d-%d", &v1, &v2);

		exist[v1] = 1;
		exist[v2] = 1;
		ch = getchar();//',' or '\n'
	}

	g.arcs[v1][v2].adj = 1;

	for (int i = 0; i < MAX_VERTEX_NUM; i++)//将存在的点对应的行列里面的数值变成0而不是max
	{
		if (exist[i])
		{
			for (int j = 0; j < MAX_VERTEX_NUM; j++)
			{
				if (g.arcs[i][j].adj != 1)
				{
					g.arcs[i][j].adj = 0;			
				}
				if (g.arcs[j][i].adj != 1)
				{
					g.arcs[j][i].adj = 0;
				}
			}
		}
		else
		{
			for (int j = 0; j < MAX_VERTEX_NUM; j++)
			{
				if (g.arcs[i][j].adj != 1)
				{
					g.arcs[i][j].adj = INFINITY;
				}
				if (g.arcs[j][i].adj != 1)
				{
					g.arcs[j][i].adj = INFINITY;
				}
			}
		}
	}

	return;
}

void InsertVertex(int iv)
{
	int i;
	int add_counter = 0;
	int p = iv;

	exist[p] = 1;
	g.vexnum++;
	for (i = 0; i < MAX_VERTEX_NUM; i++)
	{
		if (!exist[i])
			continue;
		else
		{
			g.arcs[i][p].adj = 0;
			g.arcs[p][i].adj = 0;
		}
	}
	return;
}

void InsertArc(int ia1, int ia2)
{
	int v = ia1;
	int w = ia2;

	g.arcs[v][w].adj = 1;

	return;
}

void DeleteVertex(int dv)
{
	int v = dv;

	exist[v] = 0;
	g.vexnum--;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		g.arcs[i][v].adj = INFINITY;
		g.arcs[v][i].adj = INFINITY;
	}

	return;
}

void DeleteArc(int da1, int da2)
{
	int v = da1;
	int w = da2;

	g.arcs[v][w].adj = 0;
	return;
}


void get_instruction()
{
	int tmp1, tmp2;
	int ch;
	tmp1 = -1;
	tmp2 = -1;
	ch = getchar();//ch会把指令的第一个字母给读了，因此下面的scanf里面的指令都少了一个字母
	while (ch != EOF)
	{
		if (ch == 'I')
		{
			ch = getchar();
			if (ch == 'A')//IA
			{
				scanf("(%d,%d)", &tmp1, &tmp2);
				if (tmp1 != -1 && tmp2 != -1)
				{
					inst.ia1 = tmp1;
					inst.ia2 = tmp2;
					InsertArc(inst.ia1, inst.ia2);
					ch = getchar();
					tmp1 = tmp2 = -1;
				}
			}
			else if (ch == 'V')//IV
			{
				scanf("(%d)", &tmp1);
				if (tmp1 != -1)//指令接受成功
				{
					inst.iv = tmp1;
					InsertVertex(inst.iv);
					ch = getchar();// '\n'
					tmp1 = -1;
				}
			}

		}
		else if (ch == 'D')
		{
			ch = getchar();
			if (ch == 'A')//DA
			{
				scanf("(%d,%d)", &tmp1, &tmp2);
				if (tmp1 != -1 && tmp2 != -1)
				{
					inst.da1 = tmp1;
					inst.da2 = tmp2;
					DeleteArc(inst.da1, inst.da2);
					ch = getchar();
					tmp1 = tmp2 = -1;
				}
			}
			else if (ch == 'V')//DV
			{
				scanf("(%d)", &tmp1);
				if (tmp1 != -1)//指令接受成功
				{
					inst.dv = tmp1;
					DeleteVertex(inst.dv);
					ch = getchar();// '\n'
					tmp1 = -1;
				}
			}

		}
		ch = getchar();
		if (ch != 'I' && ch != 'D')
			break;
	}
	return;
}



/*
void execute_instruction()
{
	if (inst.iv != 0)
		InsertVertex();
	if (inst.ia1 != 0 && inst.ia2 != 0)
		InsertArc();
	if (inst.dv != 0)
		DeleteVertex();
	if (inst.da1 != 0 && inst.da2 != 0)
		DeleteArc();
	return;
}
*/
void print_matrix()
{
	int index1, index2;
	int print_counter1 = 0;
	int print_counter2 = 0;
	for (index1 = 0; index1 < MAX_VERTEX_NUM; index1++)
	{
		if (!exist[index1])
			continue;
		else
		{
			print_counter1++;
			print_counter2 = 0;
		}
		for (index2 = 0; index2 < MAX_VERTEX_NUM; index2++)
		{
			if (exist[index2])
			{
				printf("%d", g.arcs[index1][index2].adj);
				print_counter2++;
				if (print_counter2 < g.vexnum)
					printf(",");				
			}
		}
		if (print_counter1 < g.vexnum)
			printf("\n");
	}
	return;
}

int main()
{
	create_cg();
	//print_matrix();

	get_instruction();
	//execute_instruction();
	print_matrix();
	return 0;
}