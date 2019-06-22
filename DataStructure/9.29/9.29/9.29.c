#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define m 20

typedef struct node
{
	struct node *nlink;
	int number;
}node;


node* h, *t;
int array[m] = { 0 };
int length = 0;
int search_array[m] = { 0 };
int length2 = 0;

void create_array()
{
	int ch;
	int number;
	do
	{
		scanf("%d", &number);
		ch = getchar();
		array[length] = number;
		length++;
	} while (ch != ';');
	
	do
	{
		scanf("%d", &number);
		ch = getchar();
		search_array[length2] = number;
		length2++;
	} while (ch != '\n' && ch != EOF);

	return;
}

void create_link()
{
	node*p, *q;
	p = q = NULL;
	h = t = NULL;
	int counter = 0;
	while (counter < length)
	{
		p = (node*)malloc(sizeof(node));
		p->number = array[counter];
		p->nlink = NULL;
		if (h == NULL)
		{
			h = p;
			q = h;
		}
		else
		{
			q->nlink = p;
			q = p;
		}

		counter++;
	}
	q->nlink = h;

	return;
}

void search()
{
	t = h;
	int search_counter = 0;
	int i;
	while (search_counter < length2)
	{
		i = search_array[search_counter];
		if (t->number < i)
		{
			while (t->number != i)
			{
				printf("%d,", t->number);
				t = t->nlink;
			}
		}
		else if (t->number > i)
		{
			printf("%d,", t->number);
			t = h;
			while (t->number != i)
			{
				printf("%d,", t->number);
				t = t->nlink;
			}
		}
		printf("%d", t->number);
		if (search_counter < length2 - 1)
			printf(";");	
		search_counter++;
	}
	return;
}

int main()
{
	create_array();
	create_link();
	search();
	return 0;
}