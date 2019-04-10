#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<stdbool.h>
#define MAXLINE 100

double atof_ver2(char s[])
{
	double val, power;
	double index = 0.0;
	double consequense;
	int i, sign, sign_of_index;
	int radix = 0;
	//bool scientific = false;
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
	
	if ((s[i] == 'E' || s[i] == 'e'))
	{
		/*
		if (isdigit(s[i + 1]) 
			||(( s[i+1] == '-'|| s[i + 1] == '+') && isdigit(s[i+2])))
			scientific = true;
		radix = s[i++];
		*/
		sign_of_index = (s[i] == '-') ? -1 : 1;
		
		if (s[i] == '+' || s[i] == '-')
			i++;
		for (index = 0.0; isdigit(s[i]); i++)
		{
			index = 10.0 * index + (s[i] - '0');
		}
		index *= sign_of_index;
	}

	consequense = (sign * val / power) * pow(10, index);
	/*
	printf("字符串中含有的数字是：%g\n", consequense );
	if (radix == 'E' && scientific)
	{
		printf("此数字还可以转换成科学计数法:%E", consequense);
	}
	else if (radix == 'e' && scientific)
	{
		printf("此数字还可以转换成科学计数法:%e", consequense);
	}
	else
	{
		;
	}
	*/

	return consequense;

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

int main()
{
	double sum, atof_ver2(char[]); 
	char line[MAXLINE];
	int getline(char line[], int max); 

	sum = 0;
	while (getline(line, MAXLINE) > 0)
		printf("\t%g\n", sum += atof_ver2(line));


	return 0;
}

