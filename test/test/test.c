#include<stdio.h>

void re_escape(char s[], char t[])
{//��s�е�ת���ַ���������
	int index;
	int status = 0;
	char c;
	char *p;
	p = t;
	for (index = 0; s[index] != '\0'; index++)
	{
		c = s[index];
		if (c == '\\' && status == 0)
		{//����Ԥ��״̬����������ܻ��е�ת���ַ�
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

int main()
{
	int a[7] = { 1, 2, 3, 4 };
	printf("%d", strlen(a));

	return 0;
}