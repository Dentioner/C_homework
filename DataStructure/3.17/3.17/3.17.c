#pragma warning(disable:4996);
#include<stdio.h>
#include<stdlib.h>

typedef struct linklist
{
	int ch;
	struct linklist* next;
	struct linklist* prior;
}node;


int main()
{
	node*ha = (node*)malloc(sizeof(node));
	node*hb = (node*)malloc(sizeof(node));
	node*now=ha;
	node*now2 = NULL;
	node*enda = NULL;
	node*endb = NULL;
	ha->prior = NULL;
	hb->prior = NULL;
	int cha;


	while ((cha = getchar()) != '&')
	{
			now->next = (node*)malloc(sizeof(node));
			now->next->prior = now;
			now->ch = cha;
			now = now->next;
	}
	enda = now->prior;
	free(enda->next);
	enda->next = NULL;
	
	now = hb;

	while ((cha = getchar()) != '@')
	{
			now->next = (node*)malloc(sizeof(node));
			now->next->prior = now;
			now->ch = cha;
			now = now->next;
	}
	endb = now->prior;
	free(endb->next);
	endb->next = NULL;

	now = ha;
	now2 = endb;
	
	while ((now != NULL) && (now2 != NULL))
	{
		if (now->ch != now2->ch)
		{
			printf("0");
			system("pause");//
			exit(0);
		}
		now = now->next;
		now2 = now2->prior;
	}

	if ((now == NULL) && (now2 == NULL))
		printf("1");
	else
		printf("0");
	system("pause");
	return 0;
}