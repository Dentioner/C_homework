#pragma warning(disable:4996);
#include<stdio.h>
#include<stdlib.h>

typedef struct nodelist
{
	int num;
	struct nodelist *prior;
	struct nodelist *next;
}node;
int main()
{
	node*head = (node*)malloc(sizeof(node));
	head->next = NULL;
	int i, ch;
	int mink, maxk;
	int length = 0;
	int length2 = 0;
	int length_origin;
	node* point, *before;
	before = head;
	do
	{
		scanf("%d", &i);
		point = (node*)malloc(sizeof(node));
		point->num = i;
		point->prior = before;
		before->next = point;
		point = point->next;
		before = before->next;
		length++;
	} while ((ch = getchar()) != '\n');
	mink = before->prior->num;
	maxk = before->num;

	point = head->next;
	while (point->num <= mink)
	{
		point = point->next;
		length2++;
	}
	before = point->prior;
	length_origin = length;
	while (point->num < maxk)
	{
		if (length2 == length_origin - 2)
			break;
		point->next->prior = before;
		point = point->next;
		free(before->next);
		before->next = point;
		
		length2++;
		length--;
	}
	point = head->next;
	for (int a = 0; a < length - 3; a++)
	{
		printf("%d ", point->num);
		point = point->next;
	}
	printf("%d", point->num);

	system("pause");//
	return 0;
}