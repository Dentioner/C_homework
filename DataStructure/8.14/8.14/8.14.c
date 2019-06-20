#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define m 10
typedef struct WORD_b
{
	struct WORD_b *llink;//前驱
	int tag;//0空闲，1占用
	int kval;//块大小，值为2的幂次k
	struct WORD_b *rlink;//后继
	int location;
}WORD_b, head;

typedef struct HeadNode
{
	int nodesize;
	
	WORD_b *first;//表头
}FreeList[m + 1];

FreeList l;
int nodenumber = 0;
void init_list()
{
	int i;
	
	for (i = 0; i < m + 1; i++)
	{
		l[i].nodesize = 1 << i;
		l[i].first = NULL;
	}
	return;
}

void create_list()
{
	int ch, location, k;
	WORD_b *p, *q, *qb;
	qb = NULL;
	ch = getchar();//k
	while (ch != '\n')
	{
		k = ch - '0';
		ch = getchar();// ' '
		while (ch != '\n')
		{
			scanf("%d", &location);
			nodenumber++;
			p = (WORD_b*)malloc(sizeof(WORD_b));
			p->kval = 1 << k;
			p->location = location;
			p->tag = 0;
			if (l[k].first == NULL)
			{
				l[k].first = p;
				l[k].first->llink = NULL;
				l[k].first->rlink = NULL;

			}
			else
			{
				q = l[k].first;
				while (q->location < p->location)
				{
					qb = q;
					q = q->rlink;
					if (q == NULL)
						break;
				}
				if (q == NULL)
				{
					qb->rlink = p;
					p->llink = qb;
					p->rlink = NULL;
				}
				else
				{
					qb->rlink = p;
					p->llink = qb;
					p->rlink = q;
					q->llink = p;
				}
			}

			ch = getchar();//' ' or '\n'
		}
		ch = getchar();// number or '\n'
	}

	return;
}

int buddy(int loc1, int loc2, int k)
{
	int bro;
	int pow_answer;
	int mod_answer;
	pow_answer = 1 << (k + 1);
	mod_answer = loc1 % pow_answer;
	if (!mod_answer)
	{
		bro = loc1 + (1 << k);
		if (bro == loc2)
			return 1;
		else
			return 0;
	}
	else
	{
		bro = loc1 - (1 << k);
		if (bro == loc2)
			return 1;
		else
			return 0;
	}
}

void merge(WORD_b *p, WORD_b *qb, int k)
{
	WORD_b *q, *qb2;
	qb2 = NULL;
	int mod_ans;
	if (l[k].first != qb)
		qb->llink = qb->rlink;
	else
		l[k].first = qb->rlink;
	p->kval = p->kval << 1;
	mod_ans = qb->location % (1 << (k + 1));
	if (!mod_ans)
	{
		p->location = qb->location;
	}
	
	if (l[k + 1].first == NULL)
	{
		l[k + 1].first = p;
		nodenumber++;
	}
	else
	{
		q = l[k + 1].first;
		qb2 = q;
		while (q->location < p->location)
		{
			qb2 = q;
			q = q->rlink;
			if (q == NULL)
				break;
		}
		if (q == NULL)
		{
			if (!buddy(qb2->location, p->location, k + 1))
			{
				qb2->rlink = p;
				p->llink = qb2;
				p->rlink = NULL;
			}
			else//合并节点
			{
				nodenumber--;
				merge(p, qb2, k + 1);
			}
		}
		else
		{
			if (buddy(qb2->location, p->location, k + 1))
			{
				nodenumber--;
				merge(p, qb2, k + 1);
			}
			else if (buddy(q->location, p->location, k + 1))
			{
				nodenumber--;
				merge(p, q, k + 1);
			
			}
			else
			{
				nodenumber++;
				if (qb2 != q)
				{
					qb2->rlink = p;
					p->llink = qb2;
					p->rlink = q;
					q->llink = p;
				}
				else
				{
					p->rlink = q;
					l[k+1].first = p;
				}
			}
		}
	}
	return;
}


void free_list()
{
	int ch, location, k;
	WORD_b *p, *q, *qb;
	qb = NULL;
	ch = getchar();//k
	while (ch >= '0' && ch <= '9')
	{
		k = ch - '0';
		ch = getchar();// ' '
		while (ch != '\n')
		{
			scanf("%d", &location);
			p = (WORD_b*)malloc(sizeof(WORD_b));
			p->kval = 1 << k;
			p->location = location;
			p->tag = 0;
			p->llink = p->rlink = NULL;
			if (l[k].first == NULL)
			{
				l[k].first = p;
				l[k].first->llink = NULL;
				l[k].first->rlink = NULL;
				nodenumber++;
			}
			else
			{
				q = l[k].first;
				qb = q;
				while (q->location < p->location)
				{
					qb = q;
					q = q->rlink;
					if (q == NULL)
						break;
				}
				if (q == NULL)
				{
					if (!buddy(qb->location, p->location, k))
					{
						qb->rlink = p;
						p->llink = qb;
						p->rlink = NULL;
						nodenumber++;
					}
					else//合并节点
					{
						nodenumber--;
						merge(p, qb, k);
					}
				}
				else
				{
					if (buddy(qb->location, p->location, k))
					{
						nodenumber--;
						merge(p, qb, k);
						
					}
					else if (buddy(q->location, p->location, k))
					{
						nodenumber--;
						merge(p, q, k);
					}
					else
					{
						nodenumber++;
						if (qb != q)
						{
							qb->rlink = p;
							p->llink = qb;
							p->rlink = q;
							q->llink = p;
						}
						else
						{
							p->rlink = q;
							l[k].first = p;
						}
					}
				}
			}

			ch = getchar();//' ' or '\n'
		}
		ch = getchar();// number or '\n'
	}
	return;
}

void print_list()
{
	int i = 0;
	WORD_b *p;
	int print_counter = 0;
	int biggest = 0;
	for (i = 0; i < m + 1; i++)
	{
		if (l[i].first != NULL)
			biggest = i;
	}
	for (i = 0; i < m + 1; i++)
	{
		if (l[i].first != NULL)
		{
			printf("%d ", i);
			p = l[i].first;
			while (p != NULL)
			{
				printf("%d", p->location);
				print_counter++;
				if (p->rlink != NULL)
					printf(" ");
				p = p->rlink;
			}
			if (i != biggest)
				printf("\n");
			
		}
	}
	return;
}

int main()
{
	init_list();
	create_list();
	free_list();
	print_list();
	return 0;
}