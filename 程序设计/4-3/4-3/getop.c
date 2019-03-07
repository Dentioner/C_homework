#include<stdio.h>
#include <ctype.h>
#include "calc.h"

/* getop: ��ȡ��һ�������������ֵ������ */
int getop(char s[])
{
	int i, c;
	int maybe_minus;
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;

	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c; /* �������� */

	i = 0;

	if (c == '-')/*��������*/
	{
		maybe_minus = getch();
		if (!isdigit(maybe_minus) && maybe_minus != '.')
			return c; /* �������� */
		else
		{ 
			s[++i] = c = maybe_minus;
			while (isdigit(s[++i] = c = getch()))
				;
		}
	}
	if (isdigit(c)) /* ��������*/
		while (isdigit(s[++i] = c = getch()))
			;

	if (c == '.') /* С������*/
		while (isdigit(s[++i] = c = getch()))
			;

	s[i] = '\0';
	if (c != EOF)
		ungetch(c);

	return NUMBER;
}




