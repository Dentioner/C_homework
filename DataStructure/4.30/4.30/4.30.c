#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 30


int* best_substr;
int length_best_substr;


int strcompare(int*a, int*b, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (a[i] != b[i])
			return 1;
	}

	return 0;
}


int find_substr_max(int*str, int slen)
{
	int*a, *b;
	int position = -1;
	int match = 0;


	for (int i = 1; 2*i <= slen; i++)//i为子串长度
	{
		match = 0;
		for (int j = 0; j + 2*i <= slen; j++)//j为子串开始的地址
		{
			a = str + j;

			for (int k = i; k + i <= slen; k++)
			{
				b = str + j + k;
				if (!strcompare(a, b, i))
				{
					best_substr = a;
					length_best_substr = i;
					position = j;
					match = 1;
					break;
				}
			}
			if (match)
				break;

		}
	}
	
	return position;
}


int main()
{
	int* str;
	int position = -1;
	int ch;
	int index = 0;

	if (!(str = (int*)malloc(LEN * sizeof(int))))
	{
		printf("fail\n");
		exit(1);
	}

	while ((ch = getchar()) != '\n')
	{
		if (index > LEN)
		{
			printf("Overflow\n");
			exit(1);
		}

		str[index++] = ch;
	}
	str[index] = '\0';

	position = find_substr_max(str, index);

	if (length_best_substr)
	{
		for (int n = 0; n < length_best_substr; n++)
			printf("%c", best_substr[n]);
		printf(" ");
	}
	printf("%d", position);
	

	system("pause");
	return 0;
}