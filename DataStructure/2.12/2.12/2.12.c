#pragma warning(disable:4996)
#define LENGTH 100
#include<stdio.h>
#include<stdlib.h>

typedef struct Link {
	int ch;
	struct Link *next;
}Node;

typedef struct head{
	int length;
	Node* first;
}head;

int main()
{
	int chch;
	int len_a =0, len_b = 0;
	head* ha, *hb;
	Node* nowa = NULL, *nowb = NULL;
	ha = (head*)malloc(sizeof(head));
	hb = (head*)malloc(sizeof(head));
	ha->first = NULL;
	hb->first = NULL;
	while ((chch = getchar()) != ';')
	{
		if (chch != ',')
		{
			if (ha->first == NULL)
			{
				ha->first = (Node*)malloc(sizeof(Node));
				ha->first->ch = chch;
				nowa = ha->first;
			}
			else
			{
				nowa->next = (Node*)malloc(sizeof(Node));
				nowa->next->ch = chch;
				nowa = nowa->next;
			}
		}
	}
	while ((chch = getchar()) != '\n')
	{
		if (chch != ',')
		{
			if (hb->first == NULL)
			{
				hb->first = (Node*)malloc(sizeof(Node));
				hb->first->ch = chch;
				nowb = hb->first;
			}
			else
			{
				nowb->next = (Node*)malloc(sizeof(Node));
				nowb->next->ch = chch;
				nowb = nowb->next;
			}
		}
	}
	
	nowa->next = NULL;
	nowb->next = NULL;

	while (ha->first!=NULL && hb->first!=NULL)
	{
		if (ha->first->ch == hb->first->ch)
		{
			nowa = ha->first;
			nowb = hb->first;
			ha->first = ha->first->next;
			hb->first = hb->first->next;
			free(nowa);
			free(nowb);
		}
		else
			break;
	}

	if (ha->first ==  NULL && hb->first == NULL)
		printf("0");
	else if (ha->first != NULL && hb->first == NULL)
		printf("2");
	else if (ha->first == NULL && hb->first != NULL)
		printf("1");
	else
	{
		if (ha->first->ch < hb->first->ch)
			printf("1");
		else
			printf("2");
	}


	system("pause");//
	return 0;
}