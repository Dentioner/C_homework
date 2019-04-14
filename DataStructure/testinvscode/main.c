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
#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'


typedef struct element
{
	int symbol;
	int priority;
}ele;

typedef struct stack
{
	ele* top;
	ele* bottom;
	int size;
}stack;




void push(stack*s, int ele);
void pop(stack*s);
int this_is_not_operator(int ch);
void compare_priority(int ch);
int get_priority(int ch);

int left_bracket = 0;//��ʾ���������ŵ��������ر��ǳ����ü��������ŵ�ʱ��

stack sheet;

int main()
{
    sheet.bottom = (ele*)malloc(STACK_SIZE_INIT * sizeof(ele));
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
			{
				left_bracket--;
				if (left_bracket < 0)
					exit(1);
			}
			else
            {
                if (sheet.bottom == sheet.top)//���ջ�ǿյģ�ֱ����ջ
					push(&sheet, ch);
                else
                {
					compare_priority(ch);

                } 
            }
            
           
        }
        
    }


	while (sheet.bottom != sheet.top)
	{
		ch = (*(sheet.top - 1)).symbol;
		printf("%c", ch);
		sheet.top--;
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
	(*(s->top)).symbol = ele;
	(*(s->top)).priority = get_priority(ele);
	s->top++;
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


void compare_priority(int input_ch)
{
	int pr_a, pr_ch;
	int a = (*(sheet.top - 1)).symbol;
	pr_a = (*(sheet.top - 1)).priority;
	pr_ch = get_priority(input_ch);
	if (pr_a >= pr_ch)
	{
		printf("%c", a);
		pop(&sheet);
		if (sheet.top != sheet.bottom)
			compare_priority(input_ch);
	}
	else
	{
		push(&sheet, input_ch);

	}
	
	return;
}

int get_priority(int ch)
{
	int pr = 0;
	if (ch == ADD || ch == SUB)
		pr = 1;
	else if (ch == MUL || ch == DIV)
		pr = 5;
	pr = pr + 10 * left_bracket;
	return pr;
}