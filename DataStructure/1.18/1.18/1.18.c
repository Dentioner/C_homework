#pragma warning(disable:4996);
#include<stdio.h>
#define arrsize 300
#define MAXINT 65535

int factorial(int n);

int main()
{
	int n;
	int answer;
	int a[arrsize] = { 0 };
	
	scanf("%d", &n);
	if (n > arrsize || n < 0)
		exit(1);
	for (int i = 0; i <= n - 1; i++)
	{
		answer = factorial(i)*(1 << i);
		if (answer > MAXINT)
		{
			printf("%d", a[i-1]);
			exit(1);
	    }
		a[i] = answer;
	}
	printf("%d", answer);
	system("pause");
	return 0;
}

int factorial(int n)
{
	int answer = 1;
	int i;
	for (i = 1; i <= n; i++)
		answer *= i;
	return answer;
}