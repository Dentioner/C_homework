#include<stdio.h>
#include<stdlib.h>

typedef struct linklist
{
	int ch;
	int number;//±àºÅ
	struct linklist* next;
	struct linklist* prior;
}node;

node*head, *now, *before;

int main()
{
	
	now = (node*)malloc(sizeof(node));
	now->next = NULL;
	now->prior = NULL;
	head = before = now;
	int ch0;
	int numnum = 0;
	int result = 1;

	while ((ch0 = getchar())!= '@')
	{
		now->ch = ch0;
		now->number = numnum;
		before = now;
		now = (node*)malloc(sizeof(node));
		now->next = NULL;
		before->next = now;
		now->prior = before;
		numnum++;
	}

	while (head->number < before->number)
	{
		if (head->ch != before->ch)
		{
			result = 0;
			break;
		}
		else
		{
			head = head->next;
			before = before->prior;
		}
	}

	printf("%d", result);

	system("pause");
	return 0;



}