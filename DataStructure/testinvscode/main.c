#include<stdio.h>
#include<stdlib.h>

/*
思路：对算符进行出栈入栈操作，调度场算法
对于一个算符，如果栈里面的算符比它的优先度高，则栈里面的算符弹出来，换它进去，并且在栈里面所有不低于它的优先度的算符一起按顺序弹出来
如果栈里面的算符的优先度比外面的低，那么外面的算符压入栈中
如果读到了左括号，那么将后续的优先度加10
如果读到了右括号，则将“优先度加10”取消
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

int left_bracket = 0;//表示出现左括号的数量，特别是出来好几个左括号的时候

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
    int in_or_out = 0;//出栈还是入栈的布尔变量

    while((ch=getchar())!='\n')
    {
        not_operator = this_is_not_operator(ch);//判断这个ch是算符还是数字
        if (not_operator)
        {
            printf("%c", ch);//是数字就print
        }
        else//是算符则考虑栈操作
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
                if (sheet.bottom == sheet.top)//如果栈是空的，直接入栈
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