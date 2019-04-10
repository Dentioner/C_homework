#include<stdio.h>

#include <stdio.h>
#define MAXLINE 1000

int getline(char line[], int max);
int strrindex(char source[], char searchfor[]);

char pattern[] = "1234";


main()
{
	char line[MAXLINE];
	int found = 0;

	while (getline(line, MAXLINE) > 0)
	{
		if (strrindex(line, pattern) >= 0)
		{
			printf("%s", line);
			found++;
		}
	}
	return found;
}


int getline(char s[], int lim)
{
	int c, i;
	i = 0;

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	if (c == '\n')
		s[i++] = c;

	s[i] = '\0';

	return i;
}


int strrindex(char s[], char t[])
{
	int i, j, k;
	/*
	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;

		if (k > 0 && t[k] == '\0')
			return i;
	}
	*/
	for (i = MAXLINE; s[i] != '\0'; i--)
		;
	i--;
	for (; i >= 0; i--)
	{
		for (j = i, k = 3; k >= 0 && s[j] == t[k]; j--, k--)
			;
		if (k < 0 && t[k + 1] == '1')
			return i - 3;//返回1的位置而不是4的位置
	}

	return -1;
}