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
	node*head, *now, *before;
	now = (node*)malloc(sizeof(node));
	now->next = NULL;
	now->prior = NULL;
	head = before = now;
	int ch0;
	while ((ch0 = getchar()) != '\n')
	{
		if (ch0 != ',')
		{
			now = (node*)malloc(sizeof(node));
			now->ch = ch0;
			before->next = now;			
			now->prior = before;
			before = now;
		}
	}

	while (now->prior != head)
	{
		printf("%c,", now->ch);
		now = now->prior;
	}
	printf("%c", now->ch);

	system("pause");
	return 0;
}