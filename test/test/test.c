#include<stdio.h>
#include<string.h>

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


int main()
{
	int a[10][2] = { {1,2},{3,4},{5,6},{7,8} };
	int b[4][2];
	memcpy(b, a, sizeof(b));
	a[2][1] = 1551;

	for (int i = 0; i < 10; i++)
	{
		myprintf(a[i], 2);
	}
	for (int i = 0; i < 4; i++)
	{
		myprintf(b[i], 2);
	}


	printf("%d", sizeof(a));
	return 0;
}