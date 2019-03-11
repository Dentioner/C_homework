#pragma warning(disable:4996);
#include<stdio.h>
#include<stdlib.h>

#define STACK_SIZE_INIT 100
#define STACK_SIZE_ADD 10



typedef struct stack
{
	int* top;
	int* bottom;
	int size;
}stack;

void push(stack*s, int ele);
void pop(stack*s);

stack sheet;

int main()
{
	
	sheet.bottom = (int*)malloc(STACK_SIZE_INIT * sizeof(int));
	
	
	
	if (sheet.bottom == NULL)
		exit(1);
	sheet.top = sheet.bottom ;
	sheet.size = STACK_SIZE_INIT;
	int ch;
	while ((ch = getchar()) != '\n')
	{
		if (ch == '(')
			push(&sheet, ch);
		else if (ch == ')')
			pop(&sheet);
	}

	if (sheet.top == sheet.bottom)
		printf("1");
	else
		printf("0");
	system("pause");
	return 0;
}

void push(stack*s, int ele)
{
	if (s->top - s->bottom >= s->size)
	{
		s = (stack*)realloc(s, sizeof(stack)*(STACK_SIZE_INIT + STACK_SIZE_ADD));
		if (s == NULL)
			exit(1);
		s->top = s->bottom + s->size;
		s->size += STACK_SIZE_ADD;
	}
	*(s->top++) = ele;
}

void pop(stack*s)
{
	if (s->top == s->bottom)
	{
		printf("0");
		system("pause");
		exit(0);
	}
	s->top--;
}