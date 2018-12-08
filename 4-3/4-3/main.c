#include<stdio.h>
#include<stdlib.h>/*供atof()函数使用*/
#include "calc.h"
#include<math.h>

#define MAXOP 100 /*运算分量或运算符的最大大小*/

int main()
{
	int type;
	double op2;
	
	char s[MAXOP];

	while ((type = getop(s)) != EOF)
	{
		if (type != NUMBER)
		{
			printf("%c ", type);
		}

		switch (type)
		{
		case NUMBER:
			printf("%s ", s);
			push(atof(s));
			break;

		case'+':
			push(pop() + pop());
			break;

		case'*':
			push(pop() * pop());
			break;

		case'-':
			op2 = pop();
			push(pop() - op2);
			break;

		case'/':
			op2 = pop();
			if (op2 != 0)
				push(pop() / op2);
			else
				printf("error:zero divisor\n");
			break;

		case'\n':
		case'\r':
			printf("\t%.8g\n", pop());
			break;
		case'%':
			op2 = pop();
			if (op2 != 0)
				push(fmod(pop(), op2));
			else
				printf("error:cannot mod zero\n");
			break;
		default:
			printf("error:unknown command %s\n", s);
			break;
		}
	}
	return 0;
}