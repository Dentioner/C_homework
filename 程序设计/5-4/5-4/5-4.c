#include<stdio.h>

int strend(char *s, char *t)
{
	int i, j;
	i = j = 0;
	while (*(s + i) != '\0')
		i++;
	//now s[i] = '\0'
	while (*(t + j) != '\0')
		j++;
	//now t[j] = '\0'
	for (; i >= 0 && j >= 0; i--, j--)
	{
		if (*(s + i) != *(t + j))
			return 0;
	}

	return 1;
}


int main()
{
	char s[] = "abcdefghijklmn";
	char t1[] = "abcdefg";
	char t2[] = "hijklmn";
	int index1, index2;
	index1 = strend(s, t1);
	index2 = strend(s, t2);

	printf("index1 = %d, index2 = %d\n", index1, index2);

	return 0;


}