#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>

#define MAXLINE 100
void re_escape(char s[], char t[])
{//将s中的转义字符隐藏起来
	int index;
	int status = 0;
	char c;
	char *p;
	p = t;
	for (index = 0; s[index] != '\0'; index++)
	{
		c = s[index];
		if (c == '\\' && status == 0)
		{//进入预备状态，检查后面可能会有的转义字符
			status = 1;
			p[index] = c;

		}

		else if (status == 1 && c == '\\')
		{
			status = 0;
			p--;
			p[index] = '\\';
		}

		else if (status == 1 && c == 'a')
		{
			status = 0;
			p--;
			p[index] = '\a';
		}

		else if (status == 1 && c == 'b')
		{
			status = 0;
			p--;
			p[index] = '\b';
		}

		else if (status == 1 && c == 'f')
		{
			status = 0;
			p--;
			p[index] = '\f';
		}
		else if (status == 1 && c == 'n')
		{
			status = 0;
			p--;
			p[index] = '\n';
		}

		else if (status == 1 && c == 'r')
		{
			status = 0;
			p--;
			p[index] = '\r';
		}
		else if (status == 1 && c == 't')
		{
			status = 0;
			p--;
			p[index] = '\t';
		}
		else if (status == 1 && c == 'v')
		{
			status = 0;
			p--;
			p[index] = '\v';
		}
		else if (status == 1 && c == '\'')
		{
			status = 0;
			p--;
			p[index] = '\'';
		}

		else if (status == 1 && c == '\"')
		{
			status = 0;
			p--;
			p[index] = '\"';
		}

		else if (status == 1 && c == '\?')
		{
			status = 0;
			p--;
			p[index] = '\?';
		}
		else if (status == 1)
		{
			status = 0;
			p[index] = c;
		}
		else
		{
			status = 0;
			p[index] = c;
		}
	}
	p[index] = '\0';
}

void myprintf(int array[], int length)
{
	int index = 0;
	for (index = 0; index < length; index++)
	{
		printf("%d,", array[index]);
	}
	printf("\n");
}


/* atof: 把字符串转化为相应的double数 */
double atof(char s[])
{
	double val, power;
	int i, sign;

	for (i = 0; isspace(s[i]); i++) /* 跳过空白符 */
		;

	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;

	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');

	if (s[i] == '.')
		i++;

	for (power = 1.0; isdigit(s[i]); i++)
	{
		val = 10.0 * val + (s[i] - '0');
		power *= 10;
	}

	return sign * val / power;
}

int getline(char s[], int lim)
{
	int c, i;
	i = 0;

	while (lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	if (c == '\n')
		s[i++] = c;

	s[i] = '\0';

	return i;
}

//利用atof的atoi
int atoi(char s[])
{
	double atof(char s[]);	//声明

	return (int)atof(s);	//调用
}


int main()
{
	double sum, atof(char[]);  //变量定义和函数声明
	char line[MAXLINE];
	int getline(char line[], int max);  //函数声明

	sum = 0;
	while (getline(line, MAXLINE) > 0)
		printf("\t%g\n", sum += atof(line));

	return 0;
}