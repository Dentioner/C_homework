#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 20


int s[MAX_VERTEX_NUM] = { 0 };
int fst[MAX_VERTEX_NUM] = { 0 };
int lst[MAX_VERTEX_NUM] = { 0 };
//注意：图的顶点从1开始，s[0], fst[0]和lst[0]都没有意义，这里都设置为0
int vexnum = 0;



void create_array()
{
	int ch;
	int index;
	int number;
	scanf("%d", &vexnum);
	ch = getchar();// '\n'
	for (index = 0; index < vexnum + 1; index++)
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		s[index] = number;
	}
	s[vexnum + 1] = '\0';

	for (index = 0; index < vexnum + 1; index++)
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		fst[index] = number;
	}
	fst[vexnum + 1] = '\0';

	for (index = 0; index < vexnum + 1; index++)
	{
		scanf("%d", &number);
		ch = getchar();// '\n' or ',' or ' '
		lst[index] = number;
	}
	lst[vexnum + 1] = '\0';

	return;
}

int main()
{
	create_array();
	return 0;
}