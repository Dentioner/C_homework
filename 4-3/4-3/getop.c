#include<stdio.h>
#include <ctype.h>
#include "calc.h"

/* getop: 获取下一个运算符或者数值操作数 */
int getop(char s[])
{
	int i, c;
	int maybe_minus;
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;

	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c; /* 不是数字 */

	i = 0;

	if (c == '-')/*负数部分*/
	{
		maybe_minus = getch();
		if (!isdigit(maybe_minus) && maybe_minus != '.')
			return c; /* 不是数字 */
		else
		{ 
			s[++i] = c = maybe_minus;
			while (isdigit(s[++i] = c = getch()))
				;
		}
	}
	if (isdigit(c)) /* 整数部分*/
		while (isdigit(s[++i] = c = getch()))
			;

	if (c == '.') /* 小数部分*/
		while (isdigit(s[++i] = c = getch()))
			;

	s[i] = '\0';
	if (c != EOF)
		ungetch(c);

	return NUMBER;
}




