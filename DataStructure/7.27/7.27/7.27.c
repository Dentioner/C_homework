#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 20
#define ERROR 0
#define OK 1
#define TRUE 1

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

typedef int QElemType;

typedef struct Node 
{
	QElemType data;
	struct Node *next;
}QNode;

typedef struct 
{
	QNode *front;
	QNode *rear;
}LinkedQueue;

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
		//v3 = v2;
		//v4 = v1;
		p = (ArcNode*)malloc(sizeof(ArcNode));
		//p2 = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = v2;
		//p2->adjvex = v4;
		p->nextarc = NULL;
		//p2->nextarc = NULL;
		index1 = 0;
		index2 = 0;
		while (g.vertices[index1].data != v1)
		{
			index1++;
		}//��һ��v1����
		/*
		while (g.vertices[index2].data != v3)
		{
			index2++;
		}//��һ��v3����
		*/
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
		/*
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
		*/
		scanf("%d-%d", &v1, &v2);
		ch = getchar();//',' or '\n'
	}

	exist[v1] = 1;
	exist[v2] = 1;
	//v3 = v2;
	//v4 = v1;
	p = (ArcNode*)malloc(sizeof(ArcNode));
	p->adjvex = v2;
	p->nextarc = NULL;
	//p2 = (ArcNode*)malloc(sizeof(ArcNode));
	//p2->adjvex = v4;
	//p2->nextarc = NULL;
	index1 = 0;
	index2 = 0;
	while (g.vertices[index1].data != v1)
	{
		index1++;
	}//��һ��v1����
	/*
	while (g.vertices[index2].data != v3)
	{
		index2++;
	}//��һ��v3����
	*/
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
	/*
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
	*/

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

int InitQueue(LinkedQueue *lq) 
{
	lq->front = lq->rear = (QNode *)malloc(sizeof(QNode));
	if (!lq->front) 
		return 0;
	lq->front->next = NULL;
	return 1;
}

int Enqueue(LinkedQueue *lq, QElemType e) 
{
	QNode *p;
	p = (QNode *)malloc(sizeof(QNode));
	if (!p) 
		return ERROR;
	p->data = e; 
	p->next = NULL;
	lq->rear->next = p; //�޸�βָ��
	lq->rear = p;
	return OK;
}

int IsQueueEmpty(LinkedQueue *lq) 
{
	if (lq->front == lq->rear)
		return 1;
	else 
		return 0;
}

int Dequeue(LinkedQueue *lq, QElemType *e) 
{
	QNode *p;
	if (lq->front == lq->rear) 
		return ERROR; //�ն��еĻ��������
	p = lq->front->next; //�޸�ͷָ��
	*e = p->data;
	lq->front->next = p->next;
	if (lq->rear == p) 
			lq->rear = lq->front;//�޸�βָ��
	free(p);
	return OK;
}

//���ض���x�ĵ�һ���ڽӶ���
int FirstAdjVex(int x) 
{
	ArcNode *p;
	p = g.vertices[x].firstarc;
	if (p) 
		return p->adjvex;
	else 
		return  -1;
}

//���ض���x��(�����y��)��һ���ڽӶ���
int NextAdjVex(int x, int y) 
{
	if (x == -1) 
		return -1;
	ArcNode *p = g.vertices[x].firstarc;
	while (p != NULL && p->adjvex != y) 
		p = p->nextarc;
	if (p != NULL && p->nextarc != NULL)
		return p->nextarc->adjvex;
	return -1;
}


void BFS()
{
	LinkedQueue Q;
	QElemType v, w, u;
	//printf("\n");//test
	int distance = 0;
	//int input_counter = 0;
	int floor_detector = 0;
	int first_input = 0;
	for (v = 0; v < MAX_VERTEX_NUM; v++)
	{
		visited[v] = 0;
	}
	InitQueue(&Q);
	//for (v = 0; v < MAX_VERTEX_NUM; v++) 
	//{
		//if (!exist[v])
			//continue;
	v = vi;
		if (!visited[v]) 
		{
			visited[v] = 1;
			//printf("-%d-", v);//����v
			Enqueue(&Q, v);//v�����
			//input_counter++;
			first_input = v;
			while (!IsQueueEmpty(&Q)) 
			{
				Dequeue(&Q, &u);
				//input_counter--;
				//if (input_counter == 0)
					//distance++;
				if (u == first_input)
					distance++;
				floor_detector = 0;
				for (w = FirstAdjVex(u); w >= 0; w = NextAdjVex(u, w))
					if (!visited[w]) 
					{
						visited[w] = TRUE;
						//printf("-%d-", w);
						if (w == vj)
							if (distance == k)
								findit = 1;
						//input_counter++;
						Enqueue(&Q, w);
						if (!floor_detector)
						{
							floor_detector = 1;
							first_input = w;
						}
						
					}//if
				//distance++;
			}//while
		}//if
	//}//for every vertex
}



int main()
{
	create_graph();
	//print_graph();
	BFS();
	if (findit)
		printf("yes");
	else
		printf("no");
	return 0;
}
