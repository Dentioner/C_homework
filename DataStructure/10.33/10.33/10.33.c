#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#define MAXLENGTH 60
#define ADDLENGTH 10


typedef struct LNode
{
	int data;
	struct LNode *next, *before;
}LNode, *LinkList;

int length_int = 0;
LinkList l = NULL;
int data_int[MAXLENGTH] = { 0 };

void create_array()
{
	int number = -1;
	int ch = ' ';
	do
	{
		scanf("%d", &number);
		ch = getchar();// ',' or '\n'
		data_int[length_int++] = number;
	} while (ch != '\n' && ch != EOF);

	return;
}


void create_link()
{
	LinkList p, q;
	p = NULL;
	l = NULL;
	q = NULL;
	int index = 0;
	while (index < length_int)
	{
		p = (LinkList)malloc(sizeof(LNode));
		p->before = p->next = NULL;
		p->data = data_int[index];
		if (l == NULL)
		{
			l = p;
			q = l;
		}
		else
		{
			p->next = q->next;
			if (q->next != NULL)
				q->next->before = p;
			q->next = p;
			p->before = q;
			q = q->next;
		}
		index++;
	}
	
	return;
}


void print_link()
{
	int counter = 0;
	LinkList p;
	p = l;
	do
	{
		printf("%d", p->data);
		counter++;
		if (counter < length_int)
			printf(",");
		p = p->next;
	} while (p != NULL);

}

void swap(int i, int j)
{
	LinkList p, q;
	int index1 = 0;
	int index2 = 0;
	int tmp = -1;
	p = q = l;
	while (index1 < i)
	{
		p = p->next;
		index1++;
	}
	while (index2 < j)
	{
		q = q->next;
		index2++;
	}

	tmp = p->data;
	p->data = q->data;
	q->data = tmp;


	return;
}

int selectMinkey(int index1)
{
	int minkey = 0;
	int i = 0;
	int index2 = 0;
		int index3 = 0;
	//int v_min, v_index;
	LinkList l_min, l_index;
	l_min = l_index = NULL;
	for (i = index1, minkey = index1; i < length_int; i++)
	{
		index2 = 0;
		l_min = l;
		while (index2 < minkey)
		{
			l_min = l_min->next;
			index2++;
		}

		index3 = 0;
		l_index = l;
		while (index3 < i)
		{
			l_index = l_index->next;
			index3++;
		}

		if (l_index->data < l_min->data)
			minkey = i;

	}

	return minkey;
}

void sort()
{
	int index1, index2;
	int tmp;
	index1 = index2 = 0;
	tmp = -1;
	for (index1 = 0; index1 < length_int; index1++)
	{
		index2 = selectMinkey(index1);
		if (index1 != index2)
			swap(index1, index2);

	}
	return;
}

int main()
{
	create_array();
	create_link();
	//print_link();
	sort();
	//printf("\n");
	print_link();
	return 0;
}
