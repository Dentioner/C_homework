#include<stdio.h>

int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	for(;low <= high;(x < v[mid])?(high = mid - 1):(low = mid + 1))
	{
		mid = (low + high) / 2;

		if (x != v[mid])
		{
			continue;

		}
		else
		{
			return mid;
		}
	}

	return -1;   
}
int main()
{
	int v[1000];
	int x;
	int ret, i;
	printf("Please input a number between 0~1000 you want to search.\n");
	scanf("%d", &x);
	for (i = 0; i < 1000; i++)	
	{
		v[i] = i;
	}

	ret = binsearch(x, v, 1000);
	if (ret != -1)
	{
		printf("the number %d is the NO.%d of the sequence.\n", x, ret);
	}
	else
	{
		printf("We cannot find the number %d", x);
	}
	return 0;
}