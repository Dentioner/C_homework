#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAX_VERTEX_NUM 20


int s[MAX_VERTEX_NUM] = { 0 };
int fst[MAX_VERTEX_NUM] = { 0 };
int lst[MAX_VERTEX_NUM] = { 0 };
//ע�⣺ͼ�Ķ����1��ʼ��s[0], fst[0]��lst[0]��û�����壬���ﶼ����Ϊ0
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