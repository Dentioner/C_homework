#include<stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);
//void bin(unsigned x);

int main()
{
	unsigned x, y;
	int p, n;
	unsigned output;
	printf("x=?\n");
	scanf("%ud", &x);
	printf("p=?\n");
	scanf("%d", &p);
	printf("n=?\n");
	scanf("%d", &n);
	printf("y=?\n");
	scanf("%ud", &y);
	output = setbits(x, p, n, y);
	printf("x = %u at first\nx = %u at last\n", x, output);

	return 0;
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	unsigned output;
	unsigned a, b;
	//假定最右边的是第0位
	//假定题目“从第p位开始的n位”想表达的意思，是从右往左数n个位，即p~p+n-1这n位
	//假定“y中最右边n位”指的是0~n-1位
	a = ~(~0 << n);
	//bin(a);
	a = ~(a << p);
	//bin(a);
	x = x & a;
	//bin(x);

	b = ~(~0 << n);
	//bin(b);
	y = y & b;
	//bin(y);

	y = y << p;
	//bin(y);
	output = x | y;
	//bin(output);
	return output;
}

/*
void bin(unsigned x)
{
	
	int i, c = 0, a[100];
	
	while (x != 0)
	{
		i = x % 2;
		a[c] = i;
		c++;
		x = x / 2;
	}
	c--;//因为c代表存入数据的长度而下表范围是0~c-1
	for (; c >= 0; c--)//倒序输出即为这个数字对应的二进制
	{
		printf("%d", a[c]);
	}
	printf("\n");
	return 0;

	
}*/