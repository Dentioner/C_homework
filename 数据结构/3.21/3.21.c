#include<stdio.h>
#include<stdlib.h>

/*
˼·����������г�ջ��ջ���������ȳ��㷨
����һ����������ջ�����������������ȶȸߣ���ջ����������������������ȥ��������ջ�������в������������ȶȵ����һ��˳�򵯳���
���ջ�������������ȶȱ�����ĵͣ���ô��������ѹ��ջ��
��������������ţ���ô�����������ȶȼ�10
��������������ţ��򽫡����ȶȼ�10��ȡ��
*/


#define STACK_SIZE_INIT 100
#define STACK_SIZE_ADD 10
#define BRACKET_PRIORITY 10


typedef struct stack
{
	int* top;
	int* bottom;
	int size;
}stack;

void push(stack*s, int ele);
void pop(stack*s);
int this_is_not_operator(int ch);
int compare_priority(int ch);
int left_bracket = 0;//��ʾ���������ŵ��������ر��ǳ����ü��������ŵ�ʱ��

stack sheet;

int main()
{
    sheet.bottom = (int*)malloc(STACK_SIZE_INIT * sizeof(int));
    if (sheet.bottom == NULL)
		exit(1);
	sheet.top = sheet.bottom ;
	sheet.size = STACK_SIZE_INIT;
	int ch;
    int not_operator = 0;
    int in_or_out = 0;//��ջ������ջ�Ĳ�������

    while((ch=getchar())!='\n')
    {
        not_operator = this_is_not_operator(ch);//�ж����ch�������������
        if (not_operator)
        {
            printf("%c", ch);//�����־�print
        }
        else//���������ջ����
        {
            if (ch == '(')
                left_bracket++;
            else if (ch == ')')
                left_bracket--;
            else
            {
                 if (sheet.bottom == sheet.top)//���ջ�ǿյģ�ֱ����ջ
                push(&sheet, ch);
                else
                {
                    //д��������
                } 
            }
            
           
        }
        
    }


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

int this_is_not_operator(int ch)
{
    if (ch >= 48 && ch <= 57)
        return 1;
    else if (ch >= 65 && ch <= 90)
        return 1;
    else if (ch >= 97 && ch <= 122)
        return 1;
    else
        return 0;
}


int compare_priority(int ch)
{
    int a;

}