#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define m 40

typedef struct WORD_b
{
	struct WORD_b *prior_link;//前驱
	int tag;//0空闲，1占用
	int kval;//块大小，
	struct WORD_b *next_link;//后继
	int location;
}block, head;

typedef struct HeadNode
{
	int lowbound;
	int highbound;
	int cellsize;
	int cell_number;
	block *first;//表头
}FreeList;

FreeList l;
int tag_array[m] = { -1 };

void create_list()
{
	int ch;
	int tag;
	int index1 = 0;
	for (int i = 0; i < m; i++)
		tag_array[i] = -1;

	scanf("%d %d %d", &(l.lowbound), &(l.highbound), &(l.cellsize));
	ch = getchar();// '\n'
	l.cell_number = (l.highbound - l.lowbound) / l.cellsize;
	ch = getchar();//
	while (ch != '\n' && ch != EOF)
	{
		tag = ch - '0';
		tag_array[index1] = tag;
		index1++;
		ch = getchar();// ' '
		ch = getchar();// '\n' or number
	}

	return;
}

void create_block()
{
	block* p, *q;
	p = q = NULL;
	l.first = NULL;
	int i;
	for (i = 0; i < l.cell_number; i++)
	{
		p = (block*)malloc(sizeof(block));
		p->tag = tag_array[i];
		p->prior_link = p->next_link = NULL;
		p->kval = l.cellsize;

		if (l.first == NULL)
		{
			l.first = p;
			p->location = l.lowbound;
		}
		else
		{
			for (q = l.first; q->next_link != NULL; q = q->next_link)
				;
			q->next_link = p;
			p->prior_link = q;
			p->location = q->location + l.cellsize;
		}

	}

	return;
}

void merge()
{
	block *p, *q;
	int counter = 0;
	p = l.first;
	q = p->next_link;
	while (q != NULL)
	{
		
		if (p->tag == 0 && q->tag == 0)
		{
			p->kval += q->kval;
			if (q->next_link != NULL)
			{
				q->next_link->prior_link = p;
				p->next_link = q->next_link;
			}
			else
			{
				p->next_link = NULL;
			}
			q = q->next_link;
		}
		else
		{
			p = p->next_link;
			q = q->next_link;
		}
	}

	return;
}

void print_block()
{
	block *p, *q, *r;
	int counter = 0;
	int counter2 = 0;
	p = l.first;
	q = NULL;
	r = l.first;
	while (r!=NULL)
	{
		if (r->tag == 0)
			counter++;
		r = r->next_link;
	}
	while (p!=NULL)
	{
		q = p->next_link;

		if (!p->tag)
		{
			printf("%d %d %d", p->tag, p->location, p->kval);
			counter2++;
			if (counter2 <counter)
				printf("\n");
		}
		p = p->next_link;
	}

	if (counter == 0)
		printf("0 0 0");
	return;
}

int main()
{
	create_list();
	create_block();
	merge();
	print_block();
	return 0;
}