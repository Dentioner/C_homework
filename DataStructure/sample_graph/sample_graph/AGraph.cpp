#include "AGraph.h"
#include "LinkedQueue.h"
#pragma warning(disable:4996)
void CreateGraph(AGraph *g, int n, int m)
{
	int i, e = 0; NodeLink *p, *q, *s; char x, y;

	g->vexnum = n; g->kind = m;
	for (i = 0; i < n; i++) {//初始化邻接表
		g->v[i].vertex = 'A' + i;
		g->v[i].first = NULL;
	}
	printf("Input edges x-->y:");
	scanf("%c%c", &x, &y);
	while (x != 'X' && y != 'X') {
		e++;
		//生成表结点并插入邻接表
		s = (NodeLink *)malloc(sizeof(NodeLink));
		s->vindex = y - 'A';
		if (g->v[x - 'A'].first == NULL) {
			g->v[x - 'A'].first = s;
			s->next = NULL;
		}
		else {
			p = g->v[x - 'A'].first;
			q = p->next;
			while (q != NULL && s->vindex > q->vindex) {
				p = q; q = q->next;
			}
			p->next = s; s->next = q;
		}
		if (!g->kind) { //无向图,即，g->kind == UDG
			s = (NodeLink *)malloc(sizeof(NodeLink));
			s->vindex = x - 'A';
			if (g->v[y - 'A'].first == NULL) {
				g->v[y - 'A'].first = s; s->next = NULL;
			}
			else {
				p = g->v[y - 'A'].first; q = p->next;
				while (q != NULL && s->vindex > q->vindex) { p = q; q = q->next; }
				p->next = s; s->next = q;
			}
		}
		scanf(" %c%c", &x, &y);
	}
	g->edgenum = e;
}

void ListGraph(AGraph *g) {
	int i; NodeLink *p;
	for (i = 0; i < g->vexnum; i++) {
		printf("%d:%c--->", i, g->v[i].vertex);
		p = g->v[i].first;
		while (p) {
			printf("%3d", p->vindex);
			p = p->next;
		}
		printf("\n");
	}
}

int LocateVex(AGraph *g, QElemType u) {
	int k;
	for (k = 0; k < g->vexnum; k++)
		if (g->v[k].vertex == u) return k;
	return -1;
}

//返回顶点x的第一个邻接顶点
int FirstAdjVex(AGraph *g, int x) {
	NodeLink *p;
	p = g->v[x].first;
	if (p) return p->vindex;
	else return  -1;
}

//返回顶点x的(相对于y的)下一个邻接顶点
int NextAdjVex(AGraph *g, int x, int y) {
	if (x == -1) return -1;
	NodeLink *p = g->v[x].first;
	while (p != NULL && p->vindex != y) p = p->next;
	if (p != NULL && p->next != NULL) return p->next->vindex;
	return -1;
}

//Non-recursive Breadth First Search
int visited[MAX_VERTEX_NUM];
void BFS(AGraph *g)
{
	LinkedQueue Q;
	QElemType v, w, u;
	for (v = 0; v < g->vexnum; v++) visited[v] = FALSE;
	InitQueue(&Q);
	for (v = 0; v < g->vexnum; v++) {
		if (!visited[v]) {
			visited[v] = TRUE;
			printf("-%d-", v);//访问v
			Enqueue(&Q, v);//v入队列
			while (!IsQueueEmpty(&Q)) {
				Dequeue(&Q, &u);
				for (w = FirstAdjVex(g, u); w >= 0; w = NextAdjVex(g, u, w))
					if (!visited[w]) {
						visited[w] = TRUE;
						printf("-%d-", w);
						Enqueue(&Q, w);
					}//if
			}//while
		}//if
	}//for every vertex
}
