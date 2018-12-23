#include<stdio.h>

void strcat(char s[], char t[])
{
	int i, j;
	i = j = 0;
	while (s[i] != '\0')
		i++;
	while ((s[i++] = t[j++]) != '\0')
		;
}

void strcat_p(char *s, char*t)
{
	int i, j;
	i = j = 0;
	while (*(s + i) != '\0')
		i++;
	while ((*(s + i) = *(t + j)) != '\0')
	{
		i++;
		j++;
	}
}

int main()
{
	char s1[100] = "abcde";
	char s2[100] = "abcde";
	char t[] = "fghjk";

	strcat(s1, t);
	strcat_p(s2, t);
	printf("use function strcat:\t");
	printf(s1);
	printf("\n");
	printf("use function strcat_p:\t");
	printf(s2);
	printf("\n");

	return 0;
}