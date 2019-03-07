#pragma warning(disable:4996)
#include<stdio.h>

int main()
{
	int a, b, c;
	int ta, tb, tc;
	int temp;
	scanf("%d %d %d", &a, &b, &c);
	if (a < b)
	{
		temp = b;
		b = a;
		a = temp;
	}
	if (c > a)
	{
		ta = c;
		tb = a;
		tc = b;
	}
	else if (c <= a && c > b)
	{
		ta = a;
		tb = c;
		tc = b;
	}
	else
	{
		ta = a;
		tb = b;
		tc = c;
	}
	printf("%d %d %d", ta, tb, tc);
	return 0;
}