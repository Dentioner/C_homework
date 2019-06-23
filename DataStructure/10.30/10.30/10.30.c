#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#define MAXLENGTH 20
#define ADDLENGTH 10

typedef struct
{
	int low;
	int high;
}bound;


typedef struct
{
	int top; //ջ��
	bound *base;
	int stacksize;
}SqStack; //��̬˳��ջ



int data_int[MAXLENGTH] = { 0 };
int length_int = 0;

void InitStack(SqStack *s)
{
	s->base = (bound*)malloc(MAXLENGTH * sizeof(bound));
	if (!s)
		exit(1);
	s->top = 0;
	s->stacksize = MAXLENGTH;
	return;
}

int GetLen(SqStack *s)
{
	return (s->top);
}



void push(SqStack *s, bound e)
{
	if (s->top >= s->stacksize)
	{ //ջ��
		s->base = (bound *)realloc(s->base, (s->stacksize + ADDLENGTH) * sizeof(bound));
		if (!s->base)
			exit(1);
		s->stacksize += ADDLENGTH;
	}
	s->base[s->top++] = e;
	return;
}

void pop(SqStack *s, bound *e)
{
	if (s->top == 0)
		exit(1);
	*e = s->base[--s->top];
	return;
}

int IsStackEmpty(SqStack *s)
{
	if (s->top == 0)
		return 1;
	else
		return 0;
}



void create_array()
{
	int number;
	int ch;
	do
	{
		scanf("%d", &number);
		ch = getchar();// ',' or '\n'
		data_int[length_int++] = number;
	} while (ch != '\n');

	return;
}

int partition(int low, int high)
{
	int pivotkey = data_int[low];
	int tmp, tmp2;
	
	
	
	while (low < high)
	{
		while (low < high && data_int[high] >= pivotkey)
			high--;
		tmp = data_int[low];
		data_int[low] = data_int[high];
		data_int[high] = tmp;

		while (low < high && data_int[low] <= pivotkey)
			low++;
		tmp = data_int[low];
		data_int[low] = data_int[high];
		data_int[high] = tmp;

	}
	
	
	return low;
}

void myQsort()
{
	SqStack s;
	int pivot_index;
	int len1, len2;
	int tmp, tmp2;
	bound p, q;
	InitStack(&s);
	p.low = 0;
	p.high = length_int - 1;
	//��ջ��ʱ������һ�ֿ���֮���ȱȽ������ֿ�ĳ��ȣ�ֻ�ѳ����Ǹ���ջ
	while (p.low < p.high || !IsStackEmpty(&s))
	{
		if (p.low < p.high)
		{
			if (p.high - p.low == 2)//�����ż�¼��<=3�����ٽ��зָ����ֱ�ӽ��бȽ�����
			{//����3���������
				tmp = data_int[p.low];
				pivot_index = p.low;
				if (data_int[p.high - 1] > data_int[p.high])//�����������ȱ�һ�´�С
				{
					tmp2 = data_int[p.high - 1];
					data_int[p.high - 1] = data_int[p.high];
					data_int[p.high] = tmp2;
				}
				if (tmp > data_int[p.high])//���������low��Ӧ��λ�õ�ֵ���
				{
					data_int[p.low] = data_int[p.high - 1];
					data_int[p.high - 1] = data_int[p.high];
					data_int[p.high] = tmp;
					pivot_index = p.high;
				}
				else if (tmp <= data_int[p.high] && tmp > data_int[p.high - 1])
				{
					data_int[p.low] = data_int[p.high - 1];
					data_int[p.high - 1] = tmp;
					pivot_index = p.high - 1;
				}
				/*
				if(!IsStackEmpty(&s))
					pop(&s, &p);
				continue;*/


			}
			else if (p.high - p.low == 1)
			{//����2���������
				pivot_index = p.low;
				if (data_int[p.low] > data_int[p.high])
				{
					tmp = data_int[p.low];
					data_int[p.low] = data_int[p.high];
					data_int[p.high] = tmp;
					pivot_index = p.high;
				}
				/*
				if(!IsStackEmpty(&s))
					pop(&s, &p);
				continue;*/
			}
			else
			{

				pivot_index = partition(p.low, p.high);
			}
				len1 = pivot_index - p.low;
				len2 = p.high - pivot_index;
				if (len1 > len2)
				{
					q.low = p.low;
					q.high = pivot_index - 1;
					push(&s, q);
					q.low = pivot_index + 1;
					q.high = p.high;
					p = q;
				}
				else
				{
					q.low = pivot_index + 1;
					q.high = p.high;
					push(&s, q);
					q.low = p.low;
					q.high = pivot_index - 1;
					p = q;
				}
			

		}

		else
		{
			pop(&s, &p);

		}
	}

	return;
}

void print_array()
{
	int index;
	for (index = 0; index < length_int; index++)
	{
		printf("%d", data_int[index]);
		if (index < length_int - 1)
			printf(",");
	}
	return;
}

int main()
{
	create_array();
	myQsort();
	print_array();
	return 0;
}