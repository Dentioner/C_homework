#include <stdio.h>
#define MAXLINE 1000 /* 最大的输入行长度 */

int getline(char line[], int max);//声明getline函数
int strindex(char source[], char searchfor[]);//声明strindex函数

char pattern[] = "1234"; /* 待查找的内容*/

/* 找出所有与待查找内容相同的行*/
main()
{
	char line[MAXLINE];
	int found = 0;

	while (getline(line, MAXLINE) > 0)
	{
		if (strindex(line, pattern) >= 0) 
		{
			printf("%s", line);
			found++;
		}
	}
	return found;
}

/* getline: 将行内容保存到s中，并返回该行的长度 */
int getline(char s[], int lim)
{
	int c, i;
	i = 0;

	while (-- lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	if (c == '\n')
		s[i++] = c;

	s[i] = '\0';
	
	return i;
}

/* strindex: 返回t在s中的位置，如果没有找到t，返回-1 */
int strindex(char s[], char t[])
{
	int i, j, k;
	for (i = 0; s[i] != '\0'; i++) 
	{
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;

		if (k > 0 && t[k] == '\0')
			return i;
	}
	return -1;
}