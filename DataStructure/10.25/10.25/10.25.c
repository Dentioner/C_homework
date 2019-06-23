#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 20

typedef struct LNode 
{
	int data;
	struct LNode *next, *before;
}LNode, *LinkList;

int data_int[MAXLENGTH] = { 0 };
int length_int = 0;
LinkList l = NULL;

void create_array()
{
	int number;
	int ch;
	do
	{
		scanf("%d", &number);
		ch = getchar();// ',' or '\n'
		data_int[length_int++] = number;
	} while (ch != '\n');

	return;
}

void create_link()
{
	LinkList p, q;
	
	int index1 = 0;
	int before;

	while (index1 < length_int)
	{
		before = 0;
		q = l;
		p = (LinkList)malloc(sizeof(LNode));
		p->before = p->next = NULL;
		p->data = data_int[index1];
		if (q == NULL)
			l = p;
		else
		{
			while (q->data < p->data && q->next != NULL)
			{
				q = q->next;
			}
			if (q->data >= p->data)
			{
				p->before = q->before;
				p->next = q;
				if(q->before != NULL)
					q->before->next = p;
				q->before = p;
			}
			else
			{
				
				p->next = q->next;
				q->next = p;
				p->before = q;
				
			}
		}
		while (l->before != NULL)
		{
			l = l->before;
		}

		index1++;
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
	} while (p!=NULL);
	
}

int main()
{
	create_array();
	create_link();
	print_link();
	return 0;
}
