#include<stdio.h>

void escape(char s[], char t[])
{//将t中的转义字符显示出来
	int index;
	char c;
	char *p;
	p = s;
	for (index = 0; t[index] != '\0'; index++)
	{
		c = t[index];
		switch (c)
		{
		case '\n':
			p[index] = '\\';
			p[index + 1] = 'n';
			p++;
			break;
		case '\t':
			p[index] = '\\';
			p[index + 1] = 't';
			p++;
			break;
		case '\v':
			p[index] = '\\';
			p[index + 1] = 'v';
			p++;
		case '\b':
			p[index] = '\\';
			p[index + 1] = 'b';
			p++;
			break;
		case '\r':
			p[index] = '\\';
			p[index + 1] = 'r';
			p++;
			break;
		case '\f':
			p[index] = '\\';
			p[index + 1] = 'f';
			p++;
			break;
		case '\a':
			p[index] = '\\';
			p[index + 1] = 'a';
			p++;
			break;

			break;
		default:
			p[index] = t[index];
			break;
		}
	}
	p[index] = '\0';
}

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
		else if(status == 1 )
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
int main()
{
	char c;
	char ch[1000];
	char s[1000] = { '\0' };
	char m[1000] = { '\0' };
	int i = 0;
	printf("请输入一段英文\n");
	while ((c = getchar()) != EOF)
	{
		ch[i] = c;
		i++;
	}
	ch[i] = '\0';

	printf("Before:\n");
	for (i = 0; ch[i] != '\0'; i++)
	{
		putchar(ch[i]);

	}
	printf("\n");

	escape(s, ch);
	printf("After:\n");

	for (i = 0; s[i] != '\0'; i++)
	{
		putchar(s[i]);

	}
	printf("\n");

	re_escape(s, m);
	printf("Last:\n");

	for (i = 0; m[i] != '\0'; i++)
	{
		putchar(m[i]);

	}
	//printf("==========================");
	printf("\n");
	//printf("==========================");
	return 0;
}