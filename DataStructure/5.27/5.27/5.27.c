#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#define MAXLENGTH 50
#define ADDLENGTH 10


typedef enum { OK, ERROR } Status;
typedef int ElemType;
typedef struct OLNode
{
	int i, j;
	ElemType e;
	struct OLNode *right, *down;
}OLNode, *OLink;

typedef struct
{
	OLink * rhead, *chead;
	int row, column, elemtotal;
}CrossList;

typedef struct
{
	ElemType *base;
	int row;
	int column;
	int elemtotal;
}Array;

Array a1, a2;

CrossList m, n;


void insert_node(int i1, int i2, int number)
{
	OLNode* p, *q;
	p = (OLNode*)malloc(sizeof(OLNode));
	p->i = i1;
	p->j = i2;
	p->e = number;
	m.elemtotal++;
	if (m.rhead[i1] == NULL || m.rhead[i1]->j > i2)
	{
		p->right = m.rhead[i1];
		m.rhead[i1] = p;
	}
	else
	{
		for (q = m.rhead[i1]; (q->right) && (q->right->j < i2); q = q->right)
			;
		p->right = q->right;
		q->right = p;
	}

	if (m.chead[i2] == NULL || m.chead[i2]->i > i1)
	{
		p->down = m.chead[i2];
		m.chead[i2] = p;
	}
	else
	{
		for (q = m.chead[i2]; (q->down) && (q->down->i < i1); q = q->down)
			;
		p->down = q->down;
		q->down = p;
	}
}

void insert_noden(int i1, int i2, int number)
{
	OLNode* p, *q;
	p = (OLNode*)malloc(sizeof(OLNode));
	p->i = i1;
	p->j = i2;
	p->e = number;
	n.elemtotal++;
	if (n.rhead[i1] == NULL || n.rhead[i1]->j > i2)
	{
		p->right = n.rhead[i1];
		n.rhead[i1] = p;
	}
	else
	{
		for (q = n.rhead[i1]; (q->right) && (q->right->j < i2); q = q->right)
			;
		p->right = q->right;
		q->right = p;
	}

	if (n.chead[i2] == NULL || n.chead[i2]->i > i1)
	{
		p->down = n.chead[i2];
		n.chead[i2] = p;
	}
	else
	{
		for (q = n.chead[i2]; (q->down) && (q->down->i < i1); q = q->down)
			;
		p->down = q->down;
		q->down = p;
	}
}

Status CreateSMatrix_OL()
{
	int row, column;
	int ch, number;
	int dim_counter = 0;
	int i1, i2;
	

	scanf("%d;%d;", &row, &column);
	m.row = n.row = row;
	m.column = n.column =  column;

	if (!(m.rhead = (OLink*)malloc((row + 1) * sizeof(OLink))))
		exit(1);
	if (!(m.chead = (OLink*)malloc((column + 1) * sizeof(OLink))))
		exit(1);
	if (!(n.rhead = (OLink*)malloc((row + 1) * sizeof(OLink))))
		exit(1);
	if (!(n.chead = (OLink*)malloc((column + 1) * sizeof(OLink))))
		exit(1);
	//m.rhead[] = m.chead[] = NULL;
	for (int p1 = 0; p1 <= m.row; p1++)
		m.rhead[p1] = NULL;
	for (int p2 = 0; p2 <= m.column; p2++)
		m.chead[p2] = NULL;
	for (int p1 = 0; p1 <= n.row; p1++)
		n.rhead[p1] = NULL;
	for (int p2 = 0; p2 <= n.column; p2++)
		n.chead[p2] = NULL;
	i1 = 0;
	ch = getchar();

	while (ch != EOF && ch != '\n' && ch != '+')
	{
		if (ch == '[')
		{
			dim_counter++;
		}
		else if (ch == ']')
		{
			dim_counter--;
			i1++;
		}


		if (dim_counter == 2)//维度计数，当[出现2次，说明到了最低维了，这一维就是数字
		{
			for (i2 = 0; i2 < m.column - 1; i2++)
			{
				scanf("%d,", &number);
				if (number != 0)
					insert_node(i1, i2, number);
				
				
			}
			
			scanf("%d", &number);
			if (number != 0)
				insert_node(i1, i2, number);
			
		}

		
		ch = getchar();
	}


	i1 = 0;
	while (ch != EOF && ch != '\n')
	{
		if (ch == '[')
		{
			dim_counter++;
		}
		else if (ch == ']')
		{
			dim_counter--;
			i1++;
		}


		if (dim_counter == 2)//维度计数，当[出现2次，说明到了最低维了，这一维就是数字
		{
			for (i2 = 0; i2 < n.column - 1; i2++)
			{
				scanf("%d,", &number);
				if (number != 0)
					insert_noden(i1, i2, number);


			}

			scanf("%d", &number);
			if (number != 0)
				insert_noden(i1, i2, number);

		}


		ch = getchar();
	}

	return OK;
}

Status InitArray()
{
	int row, column;
	int elem_max_num = MAXLENGTH;
	int ch, number;
	int dim_counter = 0;
	
	//int number_counter = 0;
	scanf("%d;%d;", &row, &column);
	a1.row = row;
	a2.row = row;
	//mx.bounds = (int*)malloc(dim * sizeof(int));
	//if (!mx.bounds)
	//	exit(1);
	a1.column = column;
	a2.column = column;
	a1.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
	a2.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
	a1.elemtotal = 0;
	a2.elemtotal = 0;
	if (!a1.base) 
		return ERROR;
	ch = getchar();
	while (ch != EOF && ch != '\n' && ch != '+')
	{
		if (ch == '[')
		{
			dim_counter++;
		}
		else if (ch == ']')
		{
			dim_counter--;
		}


		if (dim_counter == 2)//维度计数，当[出现2次，说明到了最低维了，这一维就是数字
		{
			for (int i1 = 0; i1 < a1.column - 1; i1++)
			{
				if (a1.elemtotal >= elem_max_num)
				{
					elem_max_num += ADDLENGTH;
					a1.base = (ElemType *)realloc(a1.base, sizeof(ElemType)*elem_max_num);
					//b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

					if (!a1.base)
						return ERROR;
				}
				scanf("%d,", &number);
				a1.base[a1.elemtotal++] = number;
				
			}
			if (a1.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				a1.base = (ElemType *)realloc(a1.base, sizeof(ElemType)*elem_max_num);
				
				if (!a1.base)
					return ERROR;
			}
			scanf("%d", &number);
			a1.base[a1.elemtotal++] = number;
		}


		ch = getchar();
	}

	if (a1.elemtotal >= elem_max_num)
	{
		elem_max_num += ADDLENGTH;
		a1.base = (ElemType *)realloc(a1.base, sizeof(ElemType)*elem_max_num);
		//b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

		if (!a1.base)
			return ERROR;
	}
	a1.base[a1.elemtotal] = '\0';
	//b.base[b.elemtotal] = '\0';


	while (ch != EOF && ch != '\n')
	{
		if (ch == '[')
		{
			dim_counter++;
		}
		else if (ch == ']')
		{
			dim_counter--;
		}


		if (dim_counter == 2)//维度计数，当[出现2次，说明到了最低维了，这一维就是数字
		{
			for (int i1 = 0; i1 < a2.column - 1; i1++)
			{
				if (a2.elemtotal >= elem_max_num)
				{
					elem_max_num += ADDLENGTH;
					a2.base = (ElemType *)realloc(a2.base, sizeof(ElemType)*elem_max_num);
					//b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

					if (!a2.base)
						return ERROR;
				}
				scanf("%d,", &number);
				a2.base[a2.elemtotal++] = number;

			}
			if (a2.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				a2.base = (ElemType *)realloc(a2.base, sizeof(ElemType)*elem_max_num);

				if (!a2.base)
					return ERROR;
			}
			scanf("%d", &number);
			a2.base[a2.elemtotal++] = number;
		}


		ch = getchar();
	}

	if (a2.elemtotal >= elem_max_num)
	{
		elem_max_num += ADDLENGTH;
		a2.base = (ElemType *)realloc(a2.base, sizeof(ElemType)*elem_max_num);
		//b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

		if (!a2.base)
			return ERROR;
	}
	a2.base[a2.elemtotal] = '\0';

	return OK;
}


void add_array()
{
	int index = 0;
	for (index = 0; index < a1.elemtotal; index++)
	{
		a1.base[index] += a2.base[index];
	}

	return;
}

Status printa1()
{
	int i, j;
	int index = 0;
	printf("[");
	for (i = 0; i < a1.row; i++)
	{
		printf("[");
		for (j = 0; j < a1.column - 1; j++)
		{

			printf("%d,", a1.base[index++]);
		}
		if (index != a1.elemtotal - 1)
			printf("%d];", a1.base[index++]);
		else
			printf("%d]", a1.base[index++]);
	}
	printf("]");


	return OK;
}
int main()
{
	//CreateSMatrix_OL();
	InitArray();
	add_array();
	printa1();
	system("pause");
	return 0;
}